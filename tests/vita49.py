#!/bin/env python
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK SourceVITA49.
#
# REDHAWK SourceVITA49 is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# REDHAWK SourceVITA49 is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#
import math
import struct
import time
import random
import binascii
import copy

class VITA49():
    #Options/Settings
    timestamp = None
    streamID = None
    packet_count = 0
    #CIF options
    bandwidth = None
    sample_rate = None
    rf_ref = None
    if_ref = None
    format = None
    vector_size = 1
    payload_len=256
    
    
    #Helper classes
    class PKT_TYPE:
        if_data_packet_without_stream_id = 0
        if_data_packet_with_stream_id = 1
        extention_data_packet_without_stream_id = 2
        extention_data_packet_with_stream_id = 3
        if_context_packet = 4
        extension_context_packet = 5
        
    class TSI:
        no_timestamp = 0
        utc_time = 1
        gps_time = 2
        other = 3
        
    class TSF:
        no_timestamp = 0
        sample_count = 1
        real_time = 2
        free_running = 3
        
    class CIF:
        no_fields =             0x00000000
        ctx_field_change =      0x80000000
        ref_point_id =          0x40000000
        bandwidth =             0x20000000
        if_ref_freq =           0x10000000
        rf_ref_freq =           0x08000000
        rf_ref_freq_off =       0x04000000
        if_band_off =           0x02000000
        ref_lvl =               0x01000000
        gain =                  0x00800000
        over_range_count =      0x00400000
        sample_rate =           0x00200000
        timestamp_adjust =      0x00100000
        timestamp_calibrate =   0x00080000
        temp =                  0x00040000
        dev_id =                0x00020000
        state_event_indicate =  0x00010000
        pkt_payload_fmt =       0x00008000
        fmt_gps_geo =           0x00004000
        fmt_ins_geo =           0x00002000
        ecef_ephemeris =        0x00001000
        rel_ephemeris =         0x00000800
        ephemeris_ref_id =      0x00000400
        gps_ascii =             0x00000200
        cts_assoc_list =        0x00000100
        all_fields =            0xFFFFFF00
    
    class DATA_FORMATS:
        #Formats are (complex, bytes_per_sample, V49 Type, ClassID Type)
        SB = (0, 1, 0, 2, 'b')
        CB = (1, 1, 0, 2, 'b')
        SI = (0, 2, 0, 3, 'h')
        CI = (1, 2, 0, 3, 'h')
        SL = (0, 4, 0, 4, 'i')
        CL = (1, 4, 0, 4, 'i')
        SF = (0, 4, 14, 6, 'f')
        CF = (1, 4, 14, 6, 'f')
        SD = (0, 8, 15, 7, 'd')
        CD = (1, 8, 15, 7, 'd')
    
    def generateCommonHeader(self, context=False):
        pkt = []
        #Generate header word
        hdr = 0x00000000
        
        #Determine Packet type
        #TODO: Add support for extension packets
        if not context and not self.streamID:
            hdr |= self.PKT_TYPE.if_data_packet_without_stream_id << 28
        elif not context and self.streamID:
            hdr |= self.PKT_TYPE.if_data_packet_with_stream_id << 28
        elif context:
            hdr |= self.PKT_TYPE.if_context_packet << 28
        
        #Set ClassID
        hdr |= 0x1 << 27
        
        #TODO: Add trailer support
        
        #Set timestamp bits
        #TODO: Add support for GPS time
        if self.timestamp:
            hdr |= self.TSI.utc_time << 22
            hdr |= self.TSF.real_time << 20
        else:
            hdr |= self.TSI.no_timestamp << 22
            hdr |= self.TSF.no_timestamp << 20
        
        #Set packet count
        hdr |= (self.packet_count%16) << 16
        self.packet_count += 1
        
        #Length gets set externally
        
        pkt.append(hdr)
        
        #Add Stream identifier
        if self.streamID:
            pkt.append(self.streamID)
        
        #Add ClassID
        pkt.append(0x00FFFFFA) #Well Known first work
        #2nd word is dependent on format
        cid_word = 0x00000000
        complex, width, type, cid_type, pack_type = self.format
        if complex:
            cid_word |= 1 << 20
        cid_word |= cid_type << 16
        cid_word |= self.vector_size-1
        pkt.append(cid_word)
        
        #Add in timestamps
        if self.timestamp:
            frac, sec = math.modf(self.timestamp)
            pkt.append(int(sec))
            pkt.append(int(frac*1e12) >> 32)
            pkt.append(int(frac*1e12) & 0xFFFFFFFF)
        
        return pkt
    
    def setPacketLength(self, pkt, length):
        #Set length header field post-creation
        if len(pkt) > 65536:
            raise RuntimeError("Packet is too large")
        #print "SetPacketLength ", length
        pkt[0] |= length
    
    def toVITA49Int(self, value, radix=20):
        shifted = int(value * (1 << radix) )
        upper = (shifted & 0xFFFFFFFF00000000) >> 32
        lower = shifted & 0xFFFFFFFF
        return upper, lower
    
    def generateVRLFrame(self, data=None, force_send_context=True, packet_max_elements=None, littleEndianContent=False):
        VRL_FAW = 0x56524C50
        NO_CRC = 0x56454E44
        nsamps=0
        if data : nsamps=len(data)
        
        #Generate content
        content = ""
        if force_send_context:
            content += self.generateContextPacket()
        if data != None:
            content += self.generateDataPacket(data, nsamps, littleEndianContent)
        
        #Add header
        frame = struct.pack('!I', VRL_FAW)
        frame += struct.pack('!I', len(content)/4+3)
        #Add content
        frame += content
        #Add No-CRC trailer
        frame += struct.pack('!I', NO_CRC)
        
        return frame
    
    def generateContextPacket(self, allowed_cif_fields=CIF.all_fields):
        #Generate header
        pkt = self.generateCommonHeader(context=True)
        
        #Set to no fields
        cif = self.CIF.no_fields
        context_fields = []
        
        #Check each supported field if its set and allowed
        if self.bandwidth and self.CIF.bandwidth & allowed_cif_fields != 0x0:
            cif |= self.CIF.bandwidth
            upper, lower = self.toVITA49Int(self.bandwidth)
            context_fields.append(upper)
            context_fields.append(lower)
        
        if self.if_ref and self.CIF.if_ref_freq & allowed_cif_fields != 0x0:
            cif |= self.CIF.if_ref_freq
            upper, lower = self.toVITA49Int(self.if_ref)
            context_fields.append(upper)
            context_fields.append(lower)
        
        if self.rf_ref and self.CIF.rf_ref_freq & allowed_cif_fields != 0x0:
            cif |= self.CIF.rf_ref_freq
            upper, lower = self.toVITA49Int(self.rf_ref)
            context_fields.append(upper)
            context_fields.append(lower)
        
        if self.sample_rate and self.CIF.sample_rate & allowed_cif_fields != 0x0:
            cif |= self.CIF.sample_rate
            upper, lower = self.toVITA49Int(self.sample_rate)
            context_fields.append(upper)
            context_fields.append(lower)
        
        #Always need a data format field
        cif |= self.CIF.pkt_payload_fmt
        complex, width, type, cid_type, pack_type = self.format
        format_upper = 0x00000000
        if complex:
            format_upper |= 1 << 29
        format_upper |= type << 24
        
        effective_width = width * 8 #Widths are in bits
        format_upper |= (effective_width - 1) << 6  #These are one less than the width????
        format_upper |= (effective_width - 1) #These are one less than the width????
        format_lower = 0x00000000
        format_lower |= self.vector_size - 1
        context_fields.append(format_upper)
        context_fields.append(format_lower)
        
        pkt.append(cif)
        for field in context_fields:
            pkt.append(field)
        
        self.setPacketLength(pkt, len(pkt))
        return struct.pack('!%dI'%len(pkt), *pkt)
    
    def generateDataPacket(self, data, nsamples, littleEndianContent=False):
        pkt = self.generateCommonHeader()
        
        
        content = ""
        
        #Flatten data presented as tuples
        complex, width, v49_type, cid_type, pack_type = self.format
        if complex and type(data[0]) == type(tuple()):
            #Data presented as tuples, unwind it
            flat_data = []
            for d in data:
                flat_data.append(d[0])
                flat_data.append(d[1])
        else:
            flat_data = data
        
        #Verify that we have the amount of data as indicated in the header
        if len(data) % self.vector_size != 0:
            raise RuntimeError("Sample Data "+ len(data) +" is not a multiple of VITA49 vector_size " + str(self.vector_size))
        

        #Verify we are hitting a word boundary
        if (len(flat_data)*width*8) % 32 != 0:
            raise RuntimeError("Must provide data that packs to a 32 bit boundary" )
        
        #Byte swap each element and append in
        byteOrderFlag = '!' # Network Byte Order, also Big Endian
        if littleEndianContent:
            byteOrderFlag = '<' # Little Endian
        for d in flat_data:
            #content += struct.pack('!%s'%pack_type, d)
            content += struct.pack(byteOrderFlag+'%s'%pack_type, d)
        

        #print "Data Packet format ", self.format, " samples ", nsamples, " pkt len ", len(pkt), " cnt ", len(content)
        self.setPacketLength(pkt, len(pkt) + len(content)/4)
        return struct.pack('!%dI'%len(pkt), *pkt)+content
    
    def printData(self, pkt):
        for i in range(0, len(pkt), 4):
            print "{0:4} {1}".format(i, binascii.hexlify(pkt[i:i+4]))
    
    
if __name__ == "__main__":
    v49 = VITA49() 
    v49.timestamp = time.time()
    v49.streamID = 12345
    v49.format = v49.DATA_FORMATS.SI
    v49.bandwidth = 1.0e3
    v49.sample_rate = 1.25e3
    v49.vector_size = 1
    v49.if_ref = 1.0e6
    v49.rf_ref = 2.0e9
    v94.payload_len = 256

    frame = v49.generateVRLFrame(None, force_send_context=True)
    v49.printData(frame)
    frame = v49.generateVRLFrame(xrange(0,256))
    v49.printData(frame)
    
    
    
    
    

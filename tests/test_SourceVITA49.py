#!/usr/bin/env python
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK.
#
# REDHAWK is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
#

# TODO - set MULTICAST_INTERFACE to an interface that supports multicast
MULTICAST_INTERFACE = None # 'em1' 'eth0'
DEBUG_LEVEL = 3
LITTLE_ENDIAN=1234
BIG_ENDIAN=4321

import unittest
import ossie.utils.testing
import os
from omniORB import any
from ossie.utils import sb
from ossie.utils.bulkio import bulkio_data_helpers
from bulkio.bulkioInterfaces import BULKIO, BULKIO__POA
import bulkio
from ossie.cf import CF
from bulkio import timestamp
import time
import socket
from vita49 import VITA49
from multicast import multicast_server
from unicast import unicast_server

# Full functionality is tested via end-to-end testing using SinkVITA49 and SourceVITA49 in the fulltest_VITA49.py file
class ResourceTests(ossie.utils.testing.ScaComponentTestCase):
    """Test for all resource implementations in rh.SourceVITA49"""


    def setUp(self):
        """Set up the unit test - this is run before every method that starts with test
        """
        ossie.utils.testing.ScaComponentTestCase.setUp(self)
        self.port = 29495
        self.uni_ip = '127.0.0.1'
        self.multi_ip = '236.0.10.1'
        self.vector_size=1
        self.payload_len=512
        self.componentSetup()
        self.comp.interface = "lo"
        self.comp.advanced_configuration.corba_transfersize = 100
        self.comp.advanced_configuration.buffer_size=100
        self.input_vita49_port = self.comp.getPort("dataVITA49_in")
        
        self.mserver = multicast_server(self.multi_ip, self.port)
        self.userver = unicast_server(self.uni_ip, self.port)
        
        
    def tearDown(self):
        """Finish the unit test - this is run after every method that starts with test
        """
        self.comp.stop()
        
        #######################################################################
        # Simulate regular component shutdown
        self.comp.releaseObject()
           
        ossie.utils.testing.ScaComponentTestCase.tearDown(self)
        time.sleep(1)


    def componentSetup(self):
        """Standard start-up for testing the component
        """
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will warn about using a built-in table'
        print ' * for leap seconds if no leap seconds file is found at the indicated location.'
        #######################################################################
        # Launch the component with the default execparams
        execparams = self.getPropertySet(kinds=("execparam",), modes=("readwrite", "writeonly"), includeNil=False)
        execparams = dict([(x.id, any.from_any(x.value)) for x in execparams])
        execparams["DEBUG_LEVEL"] = DEBUG_LEVEL
        self.launch(execparams, initialize=True)
        
        #######################################################################
        # Simulate regular component startup
        configureProps = self.getPropertySet(kinds=("configure",), modes=("readwrite", "writeonly"), includeNil=False)
        self.comp.configure(configureProps)
        
        # SourceVITA49 needs to be configured prior to being started, so it must be started in each unit test.
        #self.comp.start()
    
    def connectOutputData(self,portName= "dataShort_out"):
        self.dataSink = sb.DataSink()       
        self.comp.connect(self.dataSink,usesPortName=portName)

    def pushSriBigEndian(self,xdelta=1.0,streamID='TestStreamID',mode=0,kw=[]):
        kw.append(CF.DataType("dataRef", ossie.properties.to_tc_value(BIG_ENDIAN, 'string')))
        self.pushSRI(xdelta, streamID, mode, kw)
     
    def pushSriLittleEndian(self,xdelta=1.0,streamID='TestStreamID',mode=0,kw=[]):
        kw.append(CF.DataType("dataRef", ossie.properties.to_tc_value(LITTLE_ENDIAN, 'string')))
        self.pushSRI(xdelta, streamID, mode, kw)
     
    def pushSRI(self,xdelta=1.0,streamID='TestStreamID',mode=0,kw=[]):
        sri = BULKIO.StreamSRI(hversion=1, xstart=0.0, xdelta=xdelta, xunits=1, subsize=0, ystart=0.0, ydelta=0.0, yunits=0, mode=mode, streamID=streamID, blocking=False, keywords=kw)
        self.input_vita49_port.pushSRI(sri,timestamp.now())          
  

    def callAttach(self,valid_data_format=True,mode=0,streamID='id',mcast=False):
        # Mode 0 = RealShort ,  1 = ComplexShort, 2 = RealFloat, 3 = ComplexFloat
        ip = self.uni_ip
        if mcast:
            ip = self.multi_ip
        
        payloadFormat_RealShort = BULKIO.VITA49DataPacketPayloadFormat(True,BULKIO.VITA49_REAL,BULKIO.VITA49_16T,False,0,0,16,16,1,1)
        payloadFormat_ComplexShort = BULKIO.VITA49DataPacketPayloadFormat(True,BULKIO.VITA49_COMPLEX_CARTESIAN,BULKIO.VITA49_16T,False,0,0,16,16,1,1)
        payloadFormat_RealFloat = BULKIO.VITA49DataPacketPayloadFormat(True,BULKIO.VITA49_REAL,BULKIO.VITA49_32F,False,0,0,32,32,1,1)
        payloadFormat_ComplexFloat = BULKIO.VITA49DataPacketPayloadFormat(True,BULKIO.VITA49_COMPLEX_CARTESIAN,BULKIO.VITA49_32F,False,0,0,32,32,1,1)
        payloadFormat_RealLong = BULKIO.VITA49DataPacketPayloadFormat(True,BULKIO.VITA49_REAL,BULKIO.VITA49_32T,False,0,0,32,32,1,1)
        payloadFormat_RealOctet = BULKIO.VITA49DataPacketPayloadFormat(True,BULKIO.VITA49_REAL,BULKIO.VITA49_8T,False,0,0,8,8,1,1)
        
        if mode == 0:
            payloadFormat = payloadFormat_RealShort
        elif mode ==1:
            payloadFormat = payloadFormat_ComplexShort
        elif mode ==2:
            payloadFormat = payloadFormat_RealFloat
        elif mode ==3:
            payloadFormat = payloadFormat_ComplexFloat
        elif mode ==4:
            payloadFormat = payloadFormat_RealLong
        elif mode ==5:
            payloadFormat = payloadFormat_RealOctet
        else:
            payloadFormat = None
        
        streamDef = BULKIO.VITA49StreamDefinition(ip,0,self.port,BULKIO.VITA49_UDP_TRANSPORT,streamID, valid_data_format,payloadFormat)
                
        # Try to attach
        attachId = ''
        
        try:
            attachId = self.input_vita49_port.attach(streamDef, 'test') 
        except Exception, e:
            print str(e)
            print "ATTACH FAILED"
            attachId = ''
        
        self.assertTrue(attachId != '', "Failed to attach to Vita49 component")

        return attachId

    def boosthashStr(self,inputStr):
      seed = 0
      for c in inputStr:
        seed ^= (ord(c) + 0x9e3779b9 + (seed<<6) + (seed>>2)) & 0xffffffffffffffff
      return seed & 0xffffffff
            
    def createVITA49Data(self,streamID="vita49TestData",format=VITA49.DATA_FORMATS.SI,bandwidth=20e6,sample_rate=25e6,rf_ref=100e6,vector_size=1 ):
        v49 = VITA49()
        v49.timestamp = time.time()
        v49.streamID = self.boosthashStr(streamID)
        v49.format = format
        v49.bandwidth = bandwidth
        v49.sample_rate = sample_rate
        if v49.format[0]: # Complex
            v49.if_ref = 0
        else: # Real
            v49.if_ref = v49.sample_rate/4
        v49.rf_ref = rf_ref
        v49.vector_size = vector_size
        
        return v49

    def sendDataandCheck(self, vita49Frame, nsamples, littleEndianPayload=False, mcast=False, ctxFrame=None):
        
        #send_method = self.writeSocketData
        send_method = self.userver.send
        if mcast:
            send_method = self.mserver.send
        
        Indata=[x %256 for x in range(nsamples)]

        if not ctxFrame:
            ctxFrame=self.createVITA49Data('notset', 
                                      vita49Frame.format, 
                                      vita49Frame.bandwidth,
                                      vita49Frame.sample_rate,
                                      vita49Frame.rf_ref, 
                                      vita49Frame.vector_size) 
            ctxFrame.streamID = vita49Frame.streamID

        # Send Data Packet
        frame = vita49Frame.generateVRLFrame(Indata, force_send_context=False, littleEndianContent=littleEndianPayload)       
        #print "Sending frame 1 len:", len(frame)
        send_method(frame)
        time.sleep(.1)
        
        # Send Context Packet
        frame = ctxFrame.generateVRLFrame(None, force_send_context=True, littleEndianContent=littleEndianPayload)
        #print "Sending context frame "
        send_method(frame)
        time.sleep(.1)

        # Send Data Packets
        frame = vita49Frame.generateVRLFrame(Indata, force_send_context=False, littleEndianContent=littleEndianPayload)
        send_method(frame)
        #print "Sending frame 2 len:", len(frame)
        time.sleep(.1)

        vita49Frame.packet_count=1
        frame = vita49Frame.generateVRLFrame(Indata, force_send_context=False, littleEndianContent=littleEndianPayload)
        #print "Sending frame 3 len:", len(frame)
        send_method(frame)
        time.sleep(.1)

        frame = vita49Frame.generateVRLFrame(Indata, force_send_context=False, littleEndianContent=littleEndianPayload)
        #print "Sending frame 4 len:", len(frame)
        send_method(frame)
        time.sleep(.1)

        cnt=0
        scnt=0
        data=[]
        while cnt < (3*nsamples) and scnt < 4:
            tdata= self.dataSink.getData()
            if len(tdata) > 0:
                data+=tdata
                self.recv_sri=self.dataSink.sri()
            cnt += len(data)
            scnt += 1
            time.sleep(.2)

        if len(data) > 0:
            self.assertEqual(data[:nsamples], Indata[:nsamples])
        else:
            self.assertTrue(False, "Did not get Data")     

        
    def testStartStopRelease(self):
        """testStartStopRelease - Only start, stop, then release the component.
        """
        # All but start() is accomplished by the setUp and tearDown functions called for each test.
    
        #######################################################################
        # Make sure start and stop can be called without throwing exceptions
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will be unable to determine attachment'
        print ' * settings using the default configuration, and will fail to launch RX thread.'
        self.comp.start()
        #self.comp.stop()

        #######################################################################
        # Simulate regular resource shutdown
        #self.comp.releaseObject()


    def testScaBasicBehavior(self):
        """testScaBasicBehavior
        """
        #######################################################################
        # Verify the basic state of the resource
        self.assertNotEqual(self.comp, None)
        self.assertEqual(self.comp.ref._non_existent(), False)
        self.assertEqual(self.comp.ref._is_a("IDL:CF/Resource:1.0"), True)

        #######################################################################
        # Validate that query returns all expected parameters
        # Query of '[]' should return the following set of properties
        expectedProps = []
        expectedProps.extend(self.getPropertySet(kinds=("configure", "execparam"), modes=("readwrite", "readonly"), includeNil=True))
        expectedProps.extend(self.getPropertySet(kinds=("allocate",), action="external", includeNil=True))
        props = self.comp.query([])
        props = dict((x.id, any.from_any(x.value)) for x in props)
        # Query may return more than expected, but not less
        for expectedProp in expectedProps:
            self.assertEquals(props.has_key(expectedProp.id), True)

        #######################################################################
        # Verify that all expected ports are available
        for port in self.scd.get_componentfeatures().get_ports().get_uses():
            port_obj = self.comp.getPort(str(port.get_usesname()))
            self.assertNotEqual(port_obj, None)
            self.assertEqual(port_obj._non_existent(), False)
            self.assertEqual(port_obj._is_a("IDL:CF/Port:1.0"),  True)

        for port in self.scd.get_componentfeatures().get_ports().get_provides():
            port_obj = self.comp.getPort(str(port.get_providesname()))
            self.assertNotEqual(port_obj, None)
            self.assertEqual(port_obj._non_existent(), False)
            self.assertEqual(port_obj._is_a(port.get_repid()),  True)
        

    def testPortConnections(self):
        """testPortConnections
        """
        #######################################################################
        # Connect to the ports
        sink = sb.DataSink();
        self.comp.connect(sink,providesPortName='charIn')
        self.comp.connect(sink,providesPortName='octetIn')
        self.comp.connect(sink,providesPortName='shortIn')
        self.comp.connect(sink,providesPortName='ushortIn')
        self.comp.connect(sink,providesPortName='floatIn')
        self.comp.connect(sink,providesPortName='doubleIn')
        
        #######################################################################
        # Make sure start and stop can be called without throwing exceptions
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will be unable to determine attachment'
        print ' * settings using the default configuration, and will fail to launch RX thread.'

        self.comp.start()


    def testPropertyConfiguration(self):
        """testPropertyConfiguration
        """
        #######################################################################
        # Attempt to set all the properties
        self.comp.interface = "lo" 
        self.comp.attachment_override.ip_address = "127.0.0.1"
        self.comp.attachment_override.vlan = 0
        self.comp.attachment_override.use_udp_protocol = True
        self.comp.attachment_override.port = 12345
        self.comp.attachment_override.enabled = True 
        self.comp.VITA49Processing_override.VRL_frames = True
        self.comp.VITA49Processing_override.repeating = 1
        self.comp.VITA49Processing_override.event_tag_size = 0
        self.comp.VITA49Processing_override.channel_tag_size = 0
        self.comp.VITA49Processing_override.data_item_format = 5
        self.comp.VITA49Processing_override.vector_size = 1
        self.comp.VITA49Processing_override.processing_efficient = True
        self.comp.VITA49Processing_override.real_complex_type = 1
        self.comp.VITA49Processing_override.enable = True
        self.comp.advanced_configuration.vita49_packet_size = 1500
        self.comp.advanced_configuration.buffer_size = 4096000
        self.comp.advanced_configuration.poll_in_time = 1
        self.comp.advanced_configuration.corba_transfersize = 1023
        #self.comp.api() 
        
        #######################################################################
        # Make sure start and stop can be called without throwing exceptions
        print ' * Note:'
        print ' * During this unit test, SourceVITA49 will fail to connect to unicast'
        print ' * udp socket and will fail to launch the RX thread.'
        self.comp.start()

    def testAttachDetach(self):
        
        self.comp.start()
        attachId = self.callAttach(True,0)
        self.pushSriBigEndian()
        self.input_vita49_port.detach(attachId)

        attachId = self.callAttach(True,0)
        self.pushSriBigEndian()
        self.input_vita49_port.detach(attachId)
        
        attachId = self.callAttach(True,0)
        self.pushSriBigEndian()
        self.input_vita49_port.detach(attachId)


    def testSRI(self):
        
        self.connectOutputData()

        
        self.pushSriBigEndian(xdelta=.0001,mode=1,streamID="testSRIStream", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])
        attachId = self.callAttach(True,0,"testSRIStream")
        self.comp.start()

        sri = self.dataSink.sri()
        self.assertEqual(sri.streamID,"testSRIStream")
        self.assertEqual(sri.xdelta,.0001)
        self.assertEqual(sri.mode,1)
        
        expectedkws = [CF.DataType("dataRef", ossie.properties.to_tc_value(BIG_ENDIAN, 'string')),
                       CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))]

        for expectedkw in expectedkws:
            found = False
            for keyword in sri.keywords:
                if expectedkw.id==keyword.id:
                    found=True
                    self.assertEqual(expectedkw.value.value(),keyword.value.value())
                    break
            self.assertEqual(found,True)


        #Change SRI and see if changed on output
        self.pushSriBigEndian(xdelta=.01,mode=0,streamID="testSRIStream", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])
        time.sleep(.5)
        sri = self.dataSink.sri()
        self.assertEqual(sri.streamID,"testSRIStream")
        self.assertEqual(sri.xdelta,.01)
        self.assertEqual(sri.mode,0)

        # Update just a keyword
        self.pushSriBigEndian(xdelta=.01,mode=0,streamID="testSRIStream", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(200000000, 'double'))])
        time.sleep(.5)
        sri = self.dataSink.sri()

        expectedkws = [CF.DataType("dataRef", ossie.properties.to_tc_value(BIG_ENDIAN, 'string')),
                       CF.DataType("COL_RF", ossie.properties.to_tc_value(200000000, 'double'))]

        for expectedkw in expectedkws:
            found = False
            for keyword in sri.keywords:
                if expectedkw.id==keyword.id:
                    found=True
                    self.assertEqual(expectedkw.value.value(),keyword.value.value())
                    break
            self.assertEqual(found,True)


        self.input_vita49_port.detach(attachId)

  

    def testSendContextSRI(self):
        
        self.connectOutputData(portName= "dataShort_out")

        attachId = self.callAttach(True,0,"testSendDataSI")
        self.pushSriBigEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataSI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSI",format=VITA49.DATA_FORMATS.SI,vector_size=self.vector_size)
        self.recv_sri=None
        self.sendDataandCheck(v49, self.payload_len)

        self.assertNotEqual(self.recv_sri,None)
        expectedkws = [ 'COL_RF', 'COL_BW', 'COL_IF_FREQUENCY', 'USER_DEFINED',
                        'dataRef', 'CLASS_IDENTIFIER', 
                        'TimeStamp Whole Seconds','TimeStamp Fractional Seconds' ]
        recv_kws = [ x.id for x in self.recv_sri.keywords ]
        for expectedkw in expectedkws:
            msg="<"+expectedkw+"> missing from received list, kws="+str(recv_kws)
            found=(expectedkw in recv_kws)
            self.assertEqual(found,True,msg=msg)

        self.input_vita49_port.detach(attachId)    

    def testSendDataBigEndianSImcast(self):
        self.testSendDataBigEndianSI(mcast=True)

    def testSendDataBigEndianSI(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
        
        self.connectOutputData(portName= "dataShort_out")

        attachId = self.callAttach(True,0,"testSendDataSI",mcast=mcast)
        self.pushSriBigEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataSI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSI",format=VITA49.DATA_FORMATS.SI,vector_size=self.vector_size)
        self.sendDataandCheck(v49, self.payload_len, mcast=mcast)
  

        self.input_vita49_port.detach(attachId)

    def testSendDataBigEndianCImcast(self):
        self.testSendDataBigEndianCI(mcast=True)

    def testSendDataBigEndianCI(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
                
        self.connectOutputData(portName= "dataShort_out")
        attachId = self.callAttach(True,1,"testSendDataCI",mcast=mcast)
        self.pushSriBigEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataCI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataCI",format=VITA49.DATA_FORMATS.CI,vector_size=self.vector_size)
        self.sendDataandCheck(v49,self.payload_len,mcast=mcast)
  
        self.input_vita49_port.detach(attachId)
        
    def testSendDataBigEndianSFmcast(self):
        self.testSendDataBigEndianSF(mcast=True)
        
    def testSendDataBigEndianSF(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
                
        self.connectOutputData(portName= "dataFloat_out")

        attachId = self.callAttach(True,2,"testSendDataSF",mcast=mcast)
        self.pushSriBigEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataSF", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSF",format=VITA49.DATA_FORMATS.SF,vector_size=self.vector_size)
        self.sendDataandCheck(v49,self.payload_len,mcast=mcast)
  
        self.input_vita49_port.detach(attachId)
        
    def testSendDataLittleEndianSImcast(self):
        self.testSendDataLittleEndianSI(mcast=True)
        
    def testSendDataLittleEndianSI(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
        
        self.connectOutputData(portName= "dataShort_out")

        attachId = self.callAttach(True,0,"testSendDataSI",mcast=mcast)
        self.pushSriLittleEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataSI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSI",format=VITA49.DATA_FORMATS.SI,vector_size=self.vector_size)
        self.sendDataandCheck(v49, self.payload_len, littleEndianPayload=True, mcast=mcast)

        self.input_vita49_port.detach(attachId)

    def testSendDataLittleEndianCImcast(self):
        self.testSendDataLittleEndianCI(mcast=True)

    def testSendDataLittleEndianCI(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
                
        self.connectOutputData(portName= "dataShort_out")

        attachId = self.callAttach(True,1,"testSendDataCI",mcast=mcast)
        self.pushSriLittleEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataCI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataCI",format=VITA49.DATA_FORMATS.CI,vector_size=self.vector_size)
        self.sendDataandCheck(v49, self.payload_len, littleEndianPayload=True,mcast=mcast)
  
        self.input_vita49_port.detach(attachId)
        
    def testSendDataLittleEndianSFmcast(self):
        self.testSendDataLittleEndianSF(mcast=True)
        
    def testSendDataLittleEndianSF(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
                
        self.connectOutputData(portName= "dataFloat_out")

        attachId = self.callAttach(True,2,"testSendDataSF", mcast=mcast)
        self.pushSriLittleEndian(xdelta=1.0/10000,mode=1,streamID="testSendDataSF", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSF",format=VITA49.DATA_FORMATS.SF,vector_size=self.vector_size)
        self.sendDataandCheck(v49, self.payload_len, littleEndianPayload=True,mcast=mcast)

        self.input_vita49_port.detach(attachId)
        
    def testSendDataDefaultEndianSImcast(self):
        self.testSendDataDefaultEndianSI(mcast=True)
        
    def testSendDataDefaultEndianSI(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
        
        self.connectOutputData(portName= "dataShort_out")

        attachId = self.callAttach(True,0,"testSendDataSI",mcast=mcast)
        self.pushSRI(xdelta=1.0/10000,mode=1,streamID="testSendDataSI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSI",format=VITA49.DATA_FORMATS.SI,vector_size=self.vector_size)
        self.sendDataandCheck(v49, self.payload_len, mcast=mcast)

        self.input_vita49_port.detach(attachId)

    def testSendDataDefaultEndianCImcast(self):
        self.testSendDataDefaultEndianCI(mcast=True)

    def testSendDataDefaultEndianCI(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
                
        self.connectOutputData(portName= "dataShort_out")

        attachId = self.callAttach(True,1,"testSendDataCI", mcast=mcast)
        self.pushSRI(xdelta=1.0/10000,mode=1,streamID="testSendDataCI", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataCI",format=VITA49.DATA_FORMATS.CI,vector_size=self.vector_size)
        self.sendDataandCheck(v49,self.payload_len,mcast=mcast)
  
        self.input_vita49_port.detach(attachId)
        
    def testSendDataDefaultEndianSFmcast(self):
        self.testSendDataDefaultEndianSF(mcast=True)
        
    def testSendDataDefaultEndianSF(self, mcast=False):
        if mcast:
            if MULTICAST_INTERFACE is None:
                print 'WARNING - SKIPPING MULTICAST TESTS BECAUSE MULTICAST_INTERFACE IS NOT SET CORRECTLY'
                return
            self.comp.interface = MULTICAST_INTERFACE
                
        self.connectOutputData(portName= "dataFloat_out")

        attachId = self.callAttach(True,2,"testSendDataSF", mcast=mcast)
        self.pushSRI(xdelta=1.0/10000,mode=1,streamID="testSendDataSF", kw=[CF.DataType("COL_RF", ossie.properties.to_tc_value(100000000, 'double'))])

        self.comp.start()
        time.sleep(1)
        
        v49 = self.createVITA49Data(streamID="testSendDataSF",format=VITA49.DATA_FORMATS.SF,vector_size=self.vector_size)
        self.sendDataandCheck(v49, self.payload_len, mcast=mcast )

        self.input_vita49_port.detach(attachId)    
        
if __name__ == "__main__":
    ossie.utils.testing.main("../SourceVITA49.spd.xml") # By default tests all implementations

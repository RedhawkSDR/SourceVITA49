/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK core.
 *
 * REDHAWK core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.

 * This is the component code. This file contains the child class where
 * custom functionality can be added to the component. Custom
 * functionality to the base class can be extended here. Access to
 * the ports can also be done from this class
 */

#ifndef SOURCEVITA49IMPL_H
#define SOURCEVITA49IMPL_H

#include "SourceVITA49_base.h"
#include <queue>
#include <pthread.h>
#include <ossie/prop_helpers.h>
#include <ossie/Resource_impl.h>
#include <BULKIO/bulkioDataTypes.h>
#include <omniORB4/CORBA.h>
#include <omniORB4/omniURI.h>
#include <omniORB4/omniORB.h>
#include <uuid/uuid.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "VRTObject.h"
#include "BasicVRLFrame.h"
#include "BasicVRTPacket.h"
#include "StandardDataPacket.h"
#include "BasicDataPacket.h"
#include "BasicContextPacket.h"
#include "multicast.h"
#include "unicast.h"
#include "unicast_tcp.h"
#include "boost_tcp_client.h"
#include <boost/lexical_cast.hpp>
#include "BoundedBuffer.h"
#include "VITA49_struct_keywords.h"

#define CORBA_MAX_XFER_BYTES omniORB::giopMaxMsgSize() - 512
#define MTU 1500
#define DEFAULT_PORT 1001

namespace UUID_HELPER {
inline std::string new_uuid() {
    uuid_t new_random_uuid;
    uuid_generate_random(new_random_uuid);
    char new_random_uuid_str[38];
    uuid_unparse(new_random_uuid, new_random_uuid_str);
    return std::string(new_random_uuid_str);
}
}

class SourceVITA49_i : public SourceVITA49_base, public bulkio::InVITA49Port::Callback {
    ENABLE_LOGGING
public:
    ////////////////////
    // C++ Life Cycle //
    ////////////////////
    SourceVITA49_i(const char *uuid, const char *label);
    ~SourceVITA49_i();

    ////////////////////////
    // REDHAWK Life Cycle //
    ////////////////////////
    int serviceFunction();
    void start() throw (CF::Resource::StartError, CORBA::SystemException);
    void stop() throw (CF::Resource::StopError, CORBA::SystemException);

    ///////////////////////////////
    // VITA49 Callback Interface //
    ///////////////////////////////
    //std::string attach(BULKIO::VITA49StreamDefinition stream, std::string userid); // TODO: Can we remove this?
    virtual char* attach(const BULKIO::VITA49StreamDefinition& stream, const char* userid)
    throw (BULKIO::dataVITA49::AttachError, BULKIO::dataVITA49::StreamInputError);

    //void detach(std::string attachId);    // TODO: Can we remove this?
    virtual void detach(const char* attachId);

private:
    ////////////////////
    // C++ Life Cycle //
    ////////////////////
    void __constructor__();
    void initialize_values();

    //////////////////
    // Data Function//
    //////////////////
    void memoryManagement(int size);

    //////////////////////////
    // Networking Functions //
    //////////////////////////
    void RECEIVER();
    void RECEIVER_M();
    void RECEIVER_TCP();

    ///////////////////////////////
    // Property Change Listeners //
    ///////////////////////////////
    void advancedConfigurationChanged(const advanced_configuration_struct* oldVal,
            const advanced_configuration_struct* newVal);
    void attachmentOverrideChanged(const attachment_override_struct* oldVal,
            const attachment_override_struct* newVal);
    void interfacePropChanged(const std::string* oldVal,
            const std::string* newVal);
    void vita49ProcessingChanged(const VITA49Processing_override_struct* oldVal,
            const VITA49Processing_override_struct* newVal);

    ////////////////////////////
    // Time and SRI Functions //
    ////////////////////////////
    template<typename CORBAXX>
    bool addModifyKeyword(BULKIO::StreamSRI *sri, CORBA::String_member id,
            CORBAXX myValue, bool addOnly = false) {
        CORBA::Any value;
        value <<= (CORBAXX) myValue;
        unsigned long keySize = sri->keywords.length();
        if (!addOnly) {
            for (unsigned int i = 0; i < keySize; i++) {
                if (!strcmp(sri->keywords[i].id, id)) {
                    sri->keywords[i].value = value;
                    return true;
                }
            }
        }
        sri->keywords.length(keySize + 1);
        if (sri->keywords.length() != keySize + 1)
            return false;
        sri->keywords[keySize].id = CORBA::string_dup(id);
        sri->keywords[keySize].value = value;
        return true;
    }

    BULKIO::PrecisionUTCTime adjustTime(TimeStamp packet_time, bool subtract);
    bool compareSRI(BULKIO::StreamSRI A, BULKIO::StreamSRI B);
    void mergeRecSRI(const BULKIO::StreamSRI &recSRI, bool force_refresh = false);
    void newSriCallback(const BULKIO::StreamSRI &sri );

    void printSRI(BULKIO::StreamSRI *sri, std::string strHeader = "DEBUG SRI") {
        std::cout << strHeader << ":\n";
        std::cout << "\thversion: " << sri->hversion << std::endl;
        std::cout << "\txstart: " << sri->xstart << std::endl;
        std::cout << "\txdelta: " << sri->xdelta << std::endl;
        std::cout << "\txunits: " << sri->xunits << std::endl;
        std::cout << "\tsubsize: " << sri->subsize << std::endl;
        std::cout << "\tystart: " << sri->ystart << std::endl;
        std::cout << "\tydelta: " << sri->ydelta << std::endl;
        std::cout << "\tyunits: " << sri->yunits << std::endl;
        std::cout << "\tmode: " << sri->mode << std::endl;
        std::cout << "\tstreamID: " << sri->streamID << std::endl;
        for (size_t i = 0; i < sri->keywords.length(); i++) {
            std::cout << "\t KEYWORD KEY/VAL :: " << sri->keywords[i].id << ": "
                    << ossie::any_to_string(sri->keywords[i].value)
            << std::endl;
        }
        std::cout << std::endl;
    }

    void setDefaultSRI();
    void setStartOfYear();
    void sriChangedCallback(const BULKIO::StreamSRI &sri );
    double timeDiff();

    /////////////////////////
    // Threading Functions //
    /////////////////////////
    void destroy_rx_thread();
    bool launch_rx_thread();

    /////////////////////////////////
    // VITA49 Processing Functions //
    /////////////////////////////////
    struct attachment {
        bool manual_override;
        bool attach;
        std::string eth_dev;
        std::string ip_address;
        int port;
        int vlan;
        bool use_udp_protocol;
        std::string attach_id;
    };
    enum vrtTypes{
        basicVRT,
        VRLFrame,
        ContextPacket,
        DataPacket
    };

    void applyAttachSettings();
    void applyAttachSettings(attachment& attachSettings);
    bool canProcessDataPacket();
    bool hasAttachments();
    void initPayloadFormat(const BULKIO::VITA49StreamDefinition &stream_definition);
    bool isStreamDefinitionValid();

    inline void packetInfo(StandardDataPacket *pkt) {
        std::cout << pkt->getClassID() << std::endl;
        std::cout << pkt->getStreamID() << std::endl;
        std::cout << pkt->getPayloadFormat() << std::endl;
        std::cout << pkt->getTimeStamp() << std::endl;
    }

    void printStreamDef(const BULKIO::VITA49StreamDefinition& streamDef);
    void process_context(std::vector<char> *packet);
    bool process_data_packet(std::vector<char> *packet);

    void rebase_pointer_basicVRL(std::vector<char> *input_pointer){
        int offset = 0;
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer =
                (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) &(basicVRLFrame->bbuf));

        vectorPointer->_M_start = const_cast<char*>(reinterpret_cast<char*>(&((*input_pointer)[offset])));
        vectorPointer->_M_finish = vectorPointer->_M_start + (input_pointer->size());
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
    }

    void rebase_pointer_basicVRP(std::vector<char> *input_pointer){
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer =
                (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) &(basicVRPPacket->bbuf));

        vectorPointer->_M_start = const_cast<char*>(reinterpret_cast<char*>(&((*input_pointer)[_offset])));
        vectorPointer->_M_finish = vectorPointer->_M_start + (input_pointer->size()-_offset);
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
    }

    void rebase_pointer_basicVRT(std::vector<char> *input_pointer){
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer =
                (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) &(basicVRTPacket->bbuf));

        vectorPointer->_M_start = const_cast<char*>(reinterpret_cast<char*>(&((*input_pointer)[_offset])));
        vectorPointer->_M_finish = vectorPointer->_M_start + (input_pointer->size()-_offset);
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
    }

    void rebase_pointer_context(std::vector<char> *input_pointer){
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer =
                (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) &(contextPacket_g->bbuf));

        vectorPointer->_M_start = const_cast<char*>(reinterpret_cast<char*>(&((*input_pointer)[_offset])));
        vectorPointer->_M_finish = vectorPointer->_M_start + (input_pointer->size()-_offset);
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
    }

    void rebase_pointer_data(std::vector<char> *input_pointer){
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer =
                (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) &(standardDPacket->bbuf));

        vectorPointer->_M_start = const_cast<char*>(reinterpret_cast<char*>(&((*input_pointer)[_offset])));
        vectorPointer->_M_finish = vectorPointer->_M_start + (input_pointer->size()-_offset);
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
    }

    void resetAttachSettings(attachment& attachSettings);
    void resetPayloadFormat();
    void resetStreamDefinition(BULKIO::VITA49StreamDefinition& streamDef);
    bool updateAttachSettings();

    ////////////
    // BULKIO //
    ////////////
    bool _bulkioPriority;
    bool signalEOS;

    //////////
    // Data //
    //////////
//    char* array;
    bounded_buffer_deque_based< std::vector<char> *> Bank2;
    bool createMem;
    unsigned char *data;
    omni_condition *dataAvailableSignal;
    uint32_t _dataRef;
    bool init;
    int number_of_bytes;
    int numBuffers;
    int _offset;
    char *packet;
    int packetSize;
    int _readIndex;
    unsigned long transferSize;             //bytes to send in each CORBA transfer if there is not a timeout
    bounded_buffer_deque_based< std::vector<char> *> workQueue2;
    int _writeIndex;

    ///////////
    // Locks //
    ///////////
    omni_mutex dataAvailableMutex;
    boost::mutex BankLock;
    boost::mutex clientUsageLock;
    boost::mutex processing_lock;
    boost::mutex property_lock;
    boost::mutex running_lock;
    boost::mutex sriLock;
    boost::mutex startstop_lock;
    boost::mutex teardown_lock;

    ////////////////
    // Networking //
    ////////////////
    unsigned long highMulti;
    bool isMulticast;
    unsigned long lowMulti;
    multicast_t multi_client;
    bool multicast_udp_open;
    unicast_t uni_client;
    bool unicast_udp_open;
    bool unicast_tcp_open;
    unicast_tcp_t tcp_client;

    ///////////////
    // Threading //
    ///////////////
    boost::thread *_receiveThread;
    bool runThread;

    //////////////////
    // Time and SRI //
    //////////////////
    BULKIO::StreamSRI currSRI;
    bool getTimeStamp;
    BULKIO::PrecisionUTCTime lastTimeStamp;
    TimeStamp nextTimeStamp;
    bool receivedValidSRI;
    int samplesSinceLastTimeStamp;
    time_t startofyear;                     // whole seconds from EPOCH to January 1 of the current year
    std::string streamID;
    BULKIO::PrecisionUTCTime T;

    ///////////////////////
    // VITA49 Processing //
    ///////////////////////
    attachment attach_override_settings;
    attachment attach_port_settings;
    BasicVRLFrame *basicVRLFrame;
    BasicVRTPacket *basicVRPPacket;
    BasicVRTPacket *basicVRTPacket;
    int contextPacketCount;
    BasicContextPacket *contextPacket_g;
    PayloadFormat contextPayloadFormat;
    BULKIO::VITA49StreamDefinition contextStreamDefinition;
    attachment curr_attach;
    int dataPacketCount;
    bool droppedPacket;
    EPHEMERIS_ECEF_struct ephemeris_ecef_structure;
    EPHEMERIS_RELATIVE_struct ephemeris_relative_structure;
    GEOLOCATION_GPS_struct geolocation_gps_structure;
    GEOLOCATION_INS_struct geolocation_ins_structure;
    double inputSampleRate;
    bool is_input_port_attachment;
    BULKIO::VITA49StreamDefinition overrideStreamDefinition;
    bool packetDataValid;
    bool payloadFormatInitialized;
    Ephemeris processingEphemeris;
    Ephemeris processingEphemerisRel;
    Geolocation processingGEOINS;
    Geolocation processingGeolocation;
    GeoSentences processingGeoSentences;
    PayloadFormat processingPayloadFormat;
    bool receivedContextPacket;
    BasicDataPacket *standardDPacket;
    BULKIO::VITA49StreamDefinition streamDefinition;
};

#endif // SOURCEVITA49_IMPL_H

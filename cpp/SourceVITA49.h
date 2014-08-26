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
//#include "BulkioArrayPush.h"

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
    SourceVITA49_i(const char *uuid, const char *label);
    //void stop() throw (CF::Resource::StopError, CORBA::SystemException);
    void stop() throw (CF::Resource::StopError, CORBA::SystemException);
    void start() throw (CF::Resource::StartError, CORBA::SystemException);
    ~SourceVITA49_i();

    ///////////////////////////////
    //  VITA49 Callback Interface
    ///////////////////////////////
    virtual char* attach(const BULKIO::VITA49StreamDefinition& stream, const char* userid)
             throw (BULKIO::dataVITA49::AttachError, BULKIO::dataVITA49::StreamInputError);
    
    virtual void detach(const char* attachId);

    void newSriCallback(const BULKIO::StreamSRI &sri );
    
    void sriChangedCallback(const BULKIO::StreamSRI &sri );    

    int serviceFunction();

    std::string attach(BULKIO::VITA49StreamDefinition stream, std::string userid);

    void detach(std::string attachId);

    void memoryManagement(int size);

    void __constructor__();

    /* SRI handling functions */
    void setDefaultSRI();

    void mergeRecSRI(const BULKIO::StreamSRI &recSRI, bool force_refresh = false);

    bool compareSRI(BULKIO::StreamSRI A, BULKIO::StreamSRI B);

    //bool checkFS(VITApacket& packet);

    void setStartOfYear();
    /* thread function */
    void RECEIVER();
    void RECEIVER_TCP();
    void RECEIVER_M();

    // Property change listeners
    void interfacePropChanged(const std::string* oldVal, 
                              const std::string* newVal);
    
    void advancedConfigurationChanged(const advanced_configuration_struct* oldVal,
                                      const advanced_configuration_struct* newVal);
    
    void vita49ProcessingChanged(const VITA49Processing_override_struct* oldVal,
                                 const VITA49Processing_override_struct* newVal);
    
    void attachmentOverrideChanged(const attachment_override_struct* oldVal,
                                   const attachment_override_struct* newVal);
    
    bool hasAttachments();
    
private:
    boost::mutex running_lock;
    boost::mutex processing_lock;
    boost::mutex teardown_lock;
    boost::mutex startstop_lock;
    bool launch_rx_thread();
    void destroy_rx_thread();
    void initialize_values();
    bool isStreamDefinitionValid();
    bool canProcessDataPacket();
bool signalEOS;
    bool _bulkioPriority;
    uint32_t _dataRef;
    int samplesSinceLastTimeStamp;
    boost::thread* _receiveThread;
    //BULKIO::PrecisionUTCTime currentTimeStamp;
    BULKIO::PrecisionUTCTime lastTimeStamp;
    BULKIO::PrecisionUTCTime adjustTime(TimeStamp packet_time, bool subtract);
    int packetSize;
    //advanced_struct curr_advanced;
    char* packet;
    bool multicast;
    multicast_t client;
    unicast_t uni_client;
    unicast_t uni_tcp_client;
    std::string streamID;
    //bool convertEndianness;
    char* array;
    //BULKIO::PrecisionUTCTime last_timestamp;

    BULKIO::PrecisionUTCTime T;
    unsigned long transferSize; //bytes to send in each CORBA transfer if there is not a timeout
    //long long lastWSec; //holds the last number of whole seconds received
    time_t startofyear; // whole seconds from EPCH to January 1 of the current year
    bool getTimeStamp;
    int number_of_bytes;
    // Data
    //packet_struct receivedPacket;
    omni_mutex dataAvailableMutex;
    omni_condition* dataAvailableSignal;
    boost::mutex BankLock;

    int _offset;
    //
    // PingPong Queues for processing packets
    //
    bounded_buffer_deque_based< std::vector<char> *> Bank2;
    bounded_buffer_deque_based< std::vector<char> *> workQueue2;
    unsigned char* data;

    BasicContextPacket *contextPacket_g;
    BasicDataPacket *standardDPacket;
    BasicVRTPacket *basicVRTPacket;
    BasicVRTPacket *basicVRPPacket;
    BasicVRLFrame *basicVRLFrame;

    //transportMethod_override_struct curr_transport;
    int _writeIndex;
    int _readIndex;

    // SRI
    boost::mutex sriLock;
    boost::mutex property_lock;
    boost::mutex clientUsageLock;
    BULKIO::StreamSRI currSRI;
    bool is_input_port_attachment;
    bool createMem;

    double timeDiff();

    bool runThread;
    //std::ostringstream iface;
    int numBuffers;

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
            std::cout << "\t KEYWORD KEY/VAL :: " << sri->keywords[i].id << ": " << ossie::any_to_string(sri->keywords[i].value) << std::endl;
        }
        std::cout << std::endl;
    }

    template <typename CORBAXX>
    bool addModifyKeyword(BULKIO::StreamSRI *sri, CORBA::String_member id, CORBAXX myValue, bool addOnly = false) {
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
    
    BULKIO::VITA49StreamDefinition streamDefinition;
    BULKIO::VITA49StreamDefinition contextStreamDefinition;
    BULKIO::VITA49StreamDefinition overrideStreamDefinition;
    
    void process_context(std::vector<char> *packet);
    bool process_data_packet(std::vector<char> *packet);
    void resetPayloadFormat();
    void resetStreamDefinition(BULKIO::VITA49StreamDefinition& streamDef);
    
    PayloadFormat processingPayloadFormat;
    PayloadFormat contextPayloadFormat;
    double inputSampleRate;
    TimeStamp nextTimeStamp;
    Ephemeris processingEphemeris;
    Ephemeris processingEphemerisRel;
    Geolocation processingGeolocation;
    Geolocation processingGEOINS;
    GeoSentences processingGeoSentences;
    GEOLOCATION_GPS_struct geolocation_gps_structure;
    GEOLOCATION_INS_struct geolocation_ins_structure;
    EPHEMERIS_ECEF_struct ephemeris_ecef_structure;
    EPHEMERIS_RELATIVE_struct ephemeris_relative_structure;

    bool packetDataValid;
    bool payloadFormatInitialized;

    bool droppedPacket;
    bool init;

    unsigned long highMulti;
    unsigned long lowMulti;

    struct attachment {
        bool manual_override;
        bool attach;
        std::string eth_dev;
        std::string ip_address;
        int port;
        int vlan;
        bool use_udp_protocol;
        string attach_id;
    };

    int contextPacketCount;
    int dataPacketCount;

    attachment curr_attach;
    attachment attach_override_settings;
    attachment attach_port_settings;

    bool updateAttachSettings();
    void applyAttachSettings();
    void applyAttachSettings(attachment& attachSettings);
    void resetAttachSettings(attachment& attachSettings);
    
    void printStreamDef(const BULKIO::VITA49StreamDefinition& streamDef);

    void initPayloadFormat(const BULKIO::VITA49StreamDefinition &stream_definition);

    void packetInfo(StandardDataPacket *pkt) {
        std::cout << pkt->getClassID() << std::endl;
        std::cout << pkt->getStreamID() << std::endl;
        std::cout << pkt->getPayloadFormat() << std::endl;
        std::cout << pkt->getTimeStamp() << std::endl;
    }
    bool receivedValidSRI;
    bool receivedContextPacket;

    bool unicast_udp_open;
    bool unicast_tcp_open;
    bool multicast_udp_open;

    TCPClient* tcpClient;
};

#endif // SOURCEVITA49_IMPL_H

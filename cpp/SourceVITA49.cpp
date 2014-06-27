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
 *
 */
/*
 Priority of decoding methods
 * 1 - VITA49Processing_override
 * 2 - Contextual Packet
 * 3 - dataVITA49_in Attach
 */

/*
 * Priority of SRI vs Contextual Packet 
 * BULKIO_USE_PRIORITY set to 1 has higher priority than
 * a contextual packet
 */

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

 *******************************************************************************************/

#include "SourceVITA49.h"

PREPARE_LOGGING(SourceVITA49_i)

SourceVITA49_i::SourceVITA49_i(const char *uuid, const char *label) :
SourceVITA49_base(uuid, label), Bank2(numBuffers), workQueue2(numBuffers) {
    __constructor__();

    
    //dataChar_out = new OutCharArrayPort("dataChar_out");
    //dataOctet_out = new OutOctetArrayPort("dataOctet_out");
    //dataShort_out = new OutShortArrayPort("dataShort_out");
    //dataUshort_out = new OutUShortArrayPort("dataUshort_out");
    //dataFloat_out = new OutFloatArrayPort("dataFloat_out");
    //dataDouble_out = new OutDoubleArrayPort("dataDouble_out");
    
    //this->addPort("dataChar_out", dataChar_out);
    //this->addPort("dataOctet_out", dataOctet_out);
    //this->addPort("dataShort_out", dataShort_out);
    //this->addPort("dataUshort_out", dataUshort_out);
    //this->addPort("dataFloat_out", dataFloat_out);
    //this->addPort("dataDouble_out", dataDouble_out);
    
    dataVITA49_in->setNewAttachDetachCallback(this);
    dataVITA49_in->setNewSriListener(this, &SourceVITA49_i::newSriCallback);
    dataVITA49_in->setSriChangeListener(this, &SourceVITA49_i::sriChangedCallback);
    dataVITA49_in->setLogger(this->__logger);
    
    addPropertyChangeListener("interface", this, &SourceVITA49_i::interfacePropChanged);
    addPropertyChangeListener("advanced_configuration", this, &SourceVITA49_i::advancedConfigurationChanged);
    addPropertyChangeListener("VITA49Processing_override", this, &SourceVITA49_i::vita49ProcessingChanged);
    addPropertyChangeListener("attachment_override", this, &SourceVITA49_i::attachmentOverrideChanged);
}

SourceVITA49_i::~SourceVITA49_i() {
    destroy_rx_thread();
    delete basicVRLFrame;
    Bank2.clear();
    workQueue2.clear();

    if (data != NULL)
        free(data);
    if (array != NULL)
        free(array);
}

void SourceVITA49_i::__constructor__() {
    data = NULL;
    createMem = true;
    TimeStamp *temp = new TimeStamp(IntegerMode_GPS, 0, 0);
    processingEphemeris.setTimeStamp(*temp);
    processingEphemerisRel.setTimeStamp(*temp);
    processingGeolocation.setTimeStamp(*temp);
    processingGEOINS.setTimeStamp(*temp);

    delete temp;

    curr_attach.eth_dev = "eth0";
    lowMulti = inet_network("224.0.0.0");
    highMulti = inet_network("239.255.255.250");

    numBuffers = 1;
    contextPacket_g = new BasicContextPacket();
    standardDPacket = new BasicDataPacket();
    basicVRTPacket = new BasicVRTPacket();
    basicVRPPacket = new BasicVRTPacket();
    basicVRLFrame = new BasicVRLFrame(65536);
    initialize_values();
    setStartOfYear();
    
    resetAttachSettings(attach_override_settings);
    resetAttachSettings(attach_port_settings);
    
    transferSize = CORBA_MAX_XFER_BYTES;
}

void SourceVITA49_i::initialize_values() {
    createMem = true;
    multicast = false;

    contextPacketCount = 0;
    dataPacketCount = 0;
    getTimeStamp = true;

    setDefaultSRI();
    number_of_bytes = 0;
    _writeIndex = 0;
    _readIndex = 0;
    packetSize = 1500;
    _offset = 0;
    _dataRef = BYTE_ORDER;
    T.twsec = 0;
    T.tfsec = 0;
    lastTimeStamp.twsec = 0;
    lastTimeStamp.tfsec = 0;

    receivedValidSRI = false;
    receivedContextPacket = false;

    resetStreamDefinition(streamDefinition);

    connection_status.input_enabled = false;
    connection_status.input_ip_address = ossie::corba::returnString("127.0.0.1");
    connection_status.input_port = 0;
    connection_status.input_sample_rate = 0;
    connection_status.input_vlan = 0;
    connection_status.packets_missing = 0;
    //connection_status.picseconds_between_packet_timestamps =0;

    samplesSinceLastTimeStamp = 0;

    resetAttachSettings(curr_attach);
    
    inputSampleRate = 0.0;

    droppedPacket = false;
    init = true;
    array = NULL;
    _receiveThread = NULL;
    streamID.empty();
    //uni_client = NULL;
    //client = NULL;
}

void SourceVITA49_i::memoryManagement(int maxPacketLength) {
    boost::mutex::scoped_lock runLock(running_lock);
    if (data != NULL)
        free(data);
    data = (unsigned char*) malloc(CORBA_MAX_XFER_BYTES);

    if (array != NULL)
        free(array);
    array = (char*) malloc(CORBA_MAX_XFER_BYTES);

    Bank2.clear();
    workQueue2.clear();
    Bank2.set_capacity(numBuffers);
    workQueue2.set_capacity(numBuffers);

    for (int i = 0; i < numBuffers; ++i) {
        try {
            Bank2.push_front(new std::vector<char>(packetSize));
        } catch (...) {
            // we are stopping so just break out of loop
            break;
        }
    }

    createMem = false;
}

void SourceVITA49_i::newSriCallback(const BULKIO::StreamSRI &newSri) {
    LOG_DEBUG(SourceVITA49_i, "Received new sri '" << newSri.streamID << "'");
    mergeRecSRI(newSri);
    receivedValidSRI = true;
    LOG_DEBUG(SourceVITA49_i, "Handled new sri '" << newSri.streamID << "'");
}

void SourceVITA49_i::sriChangedCallback(const BULKIO::StreamSRI &newSri) {
    LOG_DEBUG(SourceVITA49_i, "Received sri change on '" << newSri.streamID << "'");
    mergeRecSRI(newSri);
    receivedValidSRI = true;
    LOG_DEBUG(SourceVITA49_i, "Handled sri change on '" << newSri.streamID << "'");
}

void SourceVITA49_i::interfacePropChanged(const std::string* oldVal, const std::string* newVal) {
    boost::mutex::scoped_lock lock(property_lock);
    interface = *newVal;
    curr_attach.eth_dev = interface;
    destroy_rx_thread();
}

void SourceVITA49_i::advancedConfigurationChanged(const advanced_configuration_struct* oldVal,
                                                  const advanced_configuration_struct* newVal) {
    boost::mutex::scoped_lock lock(property_lock);
    int temp = int (std::ceil(newVal->buffer_size / packetSize));
    numBuffers = std::max(temp, numBuffers);
    int packetSize_l = packetSize;
    packetSize = newVal->vita49_packet_size;
    if (packetSize_l != packetSize)
        createMem = true;
    transferSize = newVal->corba_transfersize;
    if (transferSize <= 0)
        transferSize = CORBA_MAX_XFER_BYTES;
    int numBuffers_l = numBuffers;
    numBuffers = int(std::max(std::ceil(newVal->buffer_size / packetSize), (double) numBuffers));
    numBuffers = int(std::max(std::ceil(transferSize / packetSize), (double) numBuffers));
    if (numBuffers_l != numBuffers)
        createMem = true;
}

void SourceVITA49_i::vita49ProcessingChanged(const VITA49Processing_override_struct* oldVal,
                                             const VITA49Processing_override_struct* newVal) {
    boost::mutex::scoped_lock lock(property_lock);
    if (newVal->enable) {
        overrideStreamDefinition.valid_data_format = true;
        if (newVal->real_complex_type == 0)
            overrideStreamDefinition.data_format.complexity = BULKIO::VITA49_REAL;
        else
            overrideStreamDefinition.data_format.complexity = BULKIO::VITA49_COMPLEX_CARTESIAN;

        switch (newVal->data_item_format) {
            case 0:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_1P;
                break;
            case 1:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_4P;
                break;
            case 2:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_8T;
                break;
            case 3:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_16T;
                break;
            case 4:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_8U;
                break;
            case 5:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_16U;
                break;
            case 6:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_32F;
                break;
            case 7:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_64F;
                break;
            case 8:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_32T;
                break;
            case 9:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_32U;
                break;
            case 10:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_64T;
                break;
            case 11:
                overrideStreamDefinition.data_format.data_item_format = BULKIO::VITA49_64U;
                break;
        }

        overrideStreamDefinition.data_format.repeating = newVal->repeating;
        overrideStreamDefinition.data_format.event_tag_size = newVal->event_tag_size;
        overrideStreamDefinition.data_format.channel_tag_size = newVal->channel_tag_size;
        overrideStreamDefinition.data_format.vector_size = newVal->vector_size;
        overrideStreamDefinition.data_format.packing_method_processing_efficient = newVal->processing_efficient;
    }
}

void SourceVITA49_i::attachmentOverrideChanged(const attachment_override_struct* oldVal,
                                               const attachment_override_struct* newVal) {
    if (attachment_override.enabled) {
        boost::mutex::scoped_lock lock(property_lock);
        if (curr_attach.attach) {
            LOG_WARN(SourceVITA49_i, "Currently attached to " << curr_attach.ip_address << " port " << curr_attach.port);
            LOG_WARN(SourceVITA49_i, "Reseting Connection ");
        }

        // Save off new attachment
        attach_override_settings.manual_override = true;
        attach_override_settings.ip_address = attachment_override.ip_address;
        attach_override_settings.vlan = attachment_override.vlan;
        attach_override_settings.port = attachment_override.port;
        attach_override_settings.eth_dev = interface;
        attach_override_settings.attach_id = UUID_HELPER::new_uuid();
        attach_override_settings.use_udp_protocol = attachment_override.use_udp_protocol;

        streamID = attach_override_settings.attach_id; //"manual_override";
        currSRI.streamID = streamID.c_str();

        destroy_rx_thread();

        //curr_transport.VRLWrapped = VITA49Processing_override.VRLWrapped;
        LOG_DEBUG(SourceVITA49_i, "Received via attach: " << curr_attach.ip_address.c_str() << " " << curr_attach.port << " " << curr_attach.vlan);

    } else {
        // Called the first time override is deactivated
        if (attach_override_settings.manual_override) {
            attach_override_settings.manual_override = false;
            destroy_rx_thread();
        }
    }
}

BULKIO::PrecisionUTCTime SourceVITA49_i::adjustTime(TimeStamp packet_time, bool subtract) {
        BULKIO::PrecisionUTCTime T_l;
        if (packet_time.getEpoch() == IntegerMode_UTC || packet_time.getEpoch() == IntegerMode_GPS) {
            T_l.tcstatus = BULKIO::TCS_VALID;
            T_l.tcmode = 0;
            T_l.twsec = packet_time.getUTCSeconds();
            T_l.tfsec = packet_time.getFractionalSeconds();
            double time = samplesSinceLastTimeStamp * (currSRI.xdelta);
            if (subtract) {
                if (time > T_l.tfsec) {
                    T_l.twsec -= ceil(time);
                    T_l.tfsec += ceil(time);
                }
                T_l.tfsec -= time;
            } else {
                T_l.tfsec += time;
                if (T_l.tfsec >= 1.0) {
                    T_l.twsec += floor(T_l.tfsec);
                    T_l.tfsec -= floor(T_l.tfsec);
                }
            }
            samplesSinceLastTimeStamp = 0;
            getTimeStamp = false;
        } else {
            T_l.tcstatus = BULKIO::TCS_INVALID;
            T_l.tcmode = 0;
            getTimeStamp = true;
        }
        return T_l;
    }

    double SourceVITA49_i::timeDiff() {
        double value = T.twsec - lastTimeStamp.twsec;
        value += (T.tfsec - lastTimeStamp.tfsec);
        lastTimeStamp.twsec = T.twsec;
        lastTimeStamp.tfsec = T.tfsec;
        return value;
    }

    int SourceVITA49_i::serviceFunction() {
        std::vector<char> *packet = NULL;

        if (createMem)
            memoryManagement(0);
        
        if (!curr_attach.attach) {
            if (!launch_rx_thread()) {
                //detach(streamID);
                LOG_WARN(SourceVITA49_i, "SourceVITA49 is unable to launch RX thread");
                serviceThread->updateDelay(1.0);
                return NOOP;
            }
            serviceThread->updateDelay(0.1);
        }

        if (!workQueue2.is_not_empty() && streamID.empty()) {
            serviceThread->updateDelay(0.1);
            return NOOP;
        }
        
        if (workQueue2.size() < transferSize / packetSize) {
            serviceThread->updateDelay(0.01);
            return NOOP;
        }

        if (streamID.empty()) {
            return NOOP;
        }
       
        unsigned int packetsDropped = 0;
        while (workQueue2.is_not_empty() && curr_attach.attach) {
            boost::mutex::scoped_lock runLock(running_lock);
            try {
                workQueue2.pop_back(&packet);
            } catch (...) {
                serviceThread->updateDelay(0.01);
                return NOOP;
            }

            connection_status.waiting_for_context_packet = !receivedContextPacket;
            connection_status.input_sample_rate = (double) round(1.0 / currSRI.xdelta);

            std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer = (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) & (basicVRPPacket->bbuf));
            vectorPointer->_M_start = const_cast<char*> ((char*) &((*packet)[_offset]));
            vectorPointer->_M_finish = vectorPointer->_M_start + (packet->size() - _offset);
            vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;

            if (basicVRPPacket->getPacketType() == PacketType_Context) {
                LOG_DEBUG(SourceVITA49_i, "RECEIVED CONTEXT PACKET");
                process_context(packet);
            }
            if (basicVRPPacket->getPacketType() == PacketType_Data) {
                LOG_DEBUG(SourceVITA49_i, "RECEIVED DATA PACKET");
                if (!process_data_packet(packet)) {
                    packetsDropped++;
                }
            }

            try {
                Bank2.push_front(packet);
            } catch (...) {
                return NORMAL;
            }
        }
        
        if (packetsDropped > 0) {
            LOG_WARN(SourceVITA49_i, "No context packet received :: Dropped Packets :: Number of packets dropped: " << packetsDropped );
        }
        
        return NORMAL;
    }

    /*******************************************************************************************
     * RECEIVER()
     *   thread function
     *
     * Takes:   void
     * Returns: void
     *
     * Functionality:
     *    Opens the connection to the multicast and then recieves packets from the multicast
     *    one at a time.  Uses the  Bank so that it does not have to keep recreateing
     *    packets.  Signals the serviceFunction every time there are enough packets in the
     *    workQueue to do a max corba transfer.
     *******************************************************************************************/
    void SourceVITA49_i::stop() throw (CF::Resource::StopError, CORBA::SystemException) {
        SourceVITA49_base::stop();

        //if (uni_client != NULL)
        unicast_close(uni_client);
        //else if (client != NULL)
        multicast_close(client);

        destroy_rx_thread();

    }

    void SourceVITA49_i::start() throw (CF::Resource::StartError, CORBA::SystemException) {
        SourceVITA49_base::start();
        sched_param myPrior;
        myPrior.__sched_priority = sched_get_priority_max(SCHED_RR);
        if (pthread_setschedparam((pthread_t) serviceThread->_mythread->native_handle(), SCHED_RR, &myPrior)) {
            LOG_INFO(SourceVITA49_i, " UNABLE TO CHANGE SCHEDULER AND PRIORITY. CHECK PERMISSIONS....");
        } else {
            LOG_DEBUG(SourceVITA49_i, " :: JUST SET SCHEDULER TO RR AND PRIORITY TO:: " << myPrior.__sched_priority << " FOR PID: " << serviceThread->_mythread->native_handle());
        }
        nice(-20);
        if (serviceThread)
            serviceThread->updateDelay(0.1);
    }

    void SourceVITA49_i::destroy_rx_thread() {
        boost::mutex::scoped_lock lock(teardown_lock);
        if (_receiveThread != NULL) {
            curr_attach.attach = false;
            runThread = false;
            _receiveThread->timed_join(boost::posix_time::seconds(1));
            delete _receiveThread;
            _receiveThread = NULL;
        }

    }
    
    bool SourceVITA49_i::updateAttachSettings() {
        if (attach_override_settings.manual_override) {
            // If override is active, use overriden properties
            applyAttachSettings(attach_override_settings);
        } else if (hasAttachments()) {
            // If we have a valid port attachment
            applyAttachSettings(attach_port_settings);
        } else {
            LOG_ERROR(SourceVITA49_i, "Unable to determine attachment settings!")
            return false;
        }
        return true;
    }

    bool SourceVITA49_i::hasAttachments() {
        BULKIO::StringSequence* attIds = dataVITA49_in->attachmentIds();
        bool hasAtts = ( attIds->length() > 0 ); 
        delete attIds;
        return hasAtts;
    }
    
    void SourceVITA49_i::applyAttachSettings(attachment& attachSettings) {
        curr_attach.attach = attachSettings.attach;
        curr_attach.ip_address = attachSettings.ip_address;
        curr_attach.manual_override = attachSettings.manual_override;
        curr_attach.port = attachSettings.port;
        curr_attach.vlan = attachSettings.vlan;
        curr_attach.use_udp_protocol = attachSettings.use_udp_protocol;
        curr_attach.attach_id = attachSettings.attach_id;
        applyAttachSettings();
    }
    
    void SourceVITA49_i::applyAttachSettings() {
        destroy_rx_thread();
        resetPayloadFormat();
    }
    
    void SourceVITA49_i::resetAttachSettings(attachment& attachSettings) {
        attachSettings.attach = false;
        attachSettings.ip_address = "127.0.0.1";
        attachSettings.manual_override = false;
        attachSettings.port = 0;
        attachSettings.vlan = 0;
        attachSettings.use_udp_protocol = true;
        attachSettings.attach_id = "";
    }
    
    bool SourceVITA49_i::launch_rx_thread() {
        LOG_TRACE(SourceVITA49_i, __PRETTY_FUNCTION__);
        destroy_rx_thread();
        runThread = true;

        // Apply/use the appropriate attachment settings
        bool validAttachSettings = updateAttachSettings();
        if (not validAttachSettings) {
            LOG_ERROR(SourceVITA49_i, "Cannot start RX_THREAD :: Invalid attachment settings!");
            return false;
        }
        
        /* build the iterface string */
        std::ostringstream iface;
        iface << curr_attach.eth_dev;

        //connect to VLAN
        if (curr_attach.vlan != 0) {
            iface << "." << curr_attach.vlan;
        }
        if (inet_network(curr_attach.ip_address.c_str()) > lowMulti && inet_addr(curr_attach.ip_address.c_str()) < highMulti && !curr_attach.ip_address.empty()) {
            LOG_DEBUG(SourceVITA49_i, "Enabling multicast_client on " << iface.str().c_str() << " " << curr_attach.ip_address.c_str() << " " << curr_attach.port);
            client = multicast_client(iface.str().c_str(), curr_attach.ip_address.c_str(), curr_attach.port);
            if (client.sock < 0) {
                LOG_ERROR(SourceVITA49_i, "Error: SourceVITA49_impl::RECEIVER() failed to connect to multicast socket");
                return false;
            }
            multicast = true;
        } else if (curr_attach.use_udp_protocol) {
            LOG_DEBUG(SourceVITA49_i, "Enabling unicast_client on " << iface.str().c_str() << " " << curr_attach.ip_address.c_str() << " " << curr_attach.port);
            uni_client = unicast_client(iface.str().c_str(), curr_attach.ip_address.c_str(), curr_attach.port);
            if (uni_client.sock < 0) {
                //std::cerr << "Error: SourceVITA49::RECEIVER() failed to connect to unicast socket  " << std::endl;
                LOG_ERROR(SourceVITA49_i, "Error: SourceVITA49::RECEIVER() failed to connect to unicast socket")
                return false;
            }
            multicast = false;
        } else {
            LOG_DEBUG(SourceVITA49_i, "Enabling unicast_client on " << iface.str().c_str() << " " << curr_attach.ip_address.c_str() << " " << curr_attach.port);
            uni_client = unicast_client(iface.str().c_str(), curr_attach.ip_address.c_str(), curr_attach.port);
            if (uni_client.sock < 0) {
                //std::cerr << "Error: SourceVITA49::RECEIVER() failed to connect to unicast socket  " << std::endl;
                LOG_ERROR(SourceVITA49_i, "Error: SourceVITA49::RECEIVER() failed to connect to unicast socket")
                return false;
            }

            multicast = false;
        }
        //update the read only properties
        connection_status.input_enabled = true;
        connection_status.input_ip_address = ossie::corba::returnString(curr_attach.ip_address.c_str());
        connection_status.input_port = curr_attach.port;
        connection_status.input_vlan = curr_attach.vlan;
        //before the thread starts, set run to true

        if (multicast)
            _receiveThread = new boost::thread(&SourceVITA49_i::RECEIVER_M, this);
        else
            _receiveThread = new boost::thread(&SourceVITA49_i::RECEIVER, this);

        LOG_DEBUG(SourceVITA49_i, "PID for receive thread is " << _receiveThread);
        /* added to increase the recieve thread priority */
        sched_param myPrior;
        myPrior.__sched_priority = sched_get_priority_max(SCHED_RR);
        if (pthread_setschedparam((pthread_t) _receiveThread->native_handle(), SCHED_RR, &myPrior)) {
            LOG_INFO(SourceVITA49_i, " UNABLE TO CHANGE SCHEDULER AND PRIORITY. CHECK PERMISSIONS..." << _receiveThread->native_handle());
        } else {
            LOG_DEBUG(SourceVITA49_i, " :: JUST SET SCHEDULER TO RR AND PRIORITY TO: " << myPrior.__sched_priority << " FOR PID: " << _receiveThread->native_handle());
        }
        nice(-20);
        curr_attach.attach = true;
        
        LOG_INFO(SourceVITA49_i, " *****************************************************************************");
        LOG_INFO(SourceVITA49_i, " **** LISTENING FOR PACKETS ON '" << curr_attach.eth_dev << "' AT " << curr_attach.ip_address << ":" << curr_attach.port);
        LOG_INFO(SourceVITA49_i, " *****************************************************************************");
        return true;
    }

    void SourceVITA49_i::RECEIVER() {
        //curr_attach.attach = true;
        std::vector<char> *packet = NULL;
        //peek at the first message and see if there is a VRL frame there
        std::vector<char> vec_char(8);
        int payloadSize = 0;
        
        //check the first packet for VRL frames
        int length = recv(uni_client.sock, &vec_char[0], 8, MSG_PEEK);
        if ((length > 0)) {
            if ((vec_char[0] == vrt::BasicVRLFrame::VRL_FAW_0) && (vec_char[1] == vrt::BasicVRLFrame::VRL_FAW_1) && (vec_char[2] == vrt::BasicVRLFrame::VRL_FAW_2) && (vec_char[3] == vrt::BasicVRLFrame::VRL_FAW_3)) {
                _offset = 8;
            } else
                _offset = 0;
        }
        
        while (runThread) {
            packet = NULL;
            // this will block until a buffer is available
            try {
                Bank2.pop_back(&packet);
            } catch (...) {
                continue;
            }

            if (runThread) payloadSize = unicast_receive(uni_client, &((*packet)[0]), packetSize, advanced_configuration.poll_in_time);
            boost::this_thread::interruption_point();

            if (payloadSize > 0) {
                //vector magic
                std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer = (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) & (basicVRTPacket->bbuf));
                vectorPointer->_M_start = const_cast<char*> (reinterpret_cast<char*> (&((*packet)[_offset])));
                vectorPointer->_M_finish = vectorPointer->_M_start + (packet->size() - _offset);
                vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
                if (basicVRTPacket->isPacketValid()) {
                    workQueue2.push_front(packet);
                } else
                    Bank2.push_front(packet);
            } else {
                usleep(100000);
                Bank2.push_front(packet);
            }

        }
    }

    void SourceVITA49_i::RECEIVER_M() {
        //curr_attach.attach = true;
        std::vector<char> *packet = NULL;
        std::vector<char> vec_char(8);
        int payloadSize = 0;
        //check the first packet for VRL frames
        int length = recv(uni_client.sock, &vec_char[0], 8, MSG_PEEK);
        if ((length > 0)) {
            if ((vec_char[0] == vrt::BasicVRLFrame::VRL_FAW_0) && (vec_char[1] == vrt::BasicVRLFrame::VRL_FAW_1) && (vec_char[2] == vrt::BasicVRLFrame::VRL_FAW_2) && (vec_char[3] == vrt::BasicVRLFrame::VRL_FAW_3)) {
                _offset = 8;
            } else
                _offset = 0;
        }

        while (runThread) {
            packet = NULL;
            //check the first packet for VRL frames
            // this will block until a buffer is available
            try {
                Bank2.pop_back(&packet);
            } catch (...) {
                continue;
            }

            if (runThread) payloadSize = multicast_receive(client, &((*packet)[0]), packetSize, advanced_configuration.poll_in_time);
            boost::this_thread::interruption_point();
            if (payloadSize > 0) {
                std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer = (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) & (basicVRTPacket->bbuf));
                vectorPointer->_M_start = const_cast<char*> (reinterpret_cast<char*> (&((*packet)[_offset])));
                vectorPointer->_M_finish = vectorPointer->_M_start + (packet->size() - _offset);
                vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;

                if (basicVRTPacket->isPacketValid()) {
                    workQueue2.push_front(packet);
                } else
                    Bank2.push_front(packet);
            } else {
                usleep(100000);
                Bank2.push_front(packet);
            }
        }
    }

    /******************************************************************************************
     * setDefaultSRI()
     *
     * Takes:   void
     * Returns: void
     *
     * Functionality:
     *
     ******************************************************************************************/
    void SourceVITA49_i::setDefaultSRI() {
        currSRI.hversion = 0;
        /* "distance" between samples (inverse of sample rate) */
        currSRI.xdelta = (double) 0;
        /* 0 for Scalar, 1 for Complex */
        currSRI.mode = (short) 0;

        currSRI.streamID = "DEFAULT_SOURCEVITA49_STREAMID";
        currSRI.blocking = false;
        currSRI.hversion = (long) 0;
        currSRI.xstart = (double) 0;

        /* Platinum time code (1 == seconds) */
        currSRI.xunits = (short) 1;

        /* # frames to be delivered by pushPacket() call; set to 0 for single packet */
        currSRI.ystart = (double) 0;
        currSRI.ydelta = (double) 0.001;
        currSRI.yunits = (short) 1;
        currSRI.subsize = 0;
        currSRI.keywords.length(0);

    }

    void SourceVITA49_i::initPayloadFormat(const BULKIO::VITA49StreamDefinition & stream_definition) {
        processingPayloadFormat.setProcessingEfficient(stream_definition.data_format.packing_method_processing_efficient);

        //set complexity
        if (stream_definition.data_format.complexity == BULKIO::VITA49_REAL) {
            processingPayloadFormat.setRealComplexType(RealComplexType_Real);
        } else if (stream_definition.data_format.complexity == BULKIO::VITA49_COMPLEX_CARTESIAN) {
            processingPayloadFormat.setRealComplexType(RealComplexType_ComplexCartesian);
        } else {
            processingPayloadFormat.setRealComplexType(RealComplexType_reserved3);
        }

        //set the data type
        if (stream_definition.data_format.data_item_format == BULKIO::VITA49_1P)
            processingPayloadFormat.setDataType(DataType_UInt1);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_4P)
            processingPayloadFormat.setDataType(DataType_UInt4);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_8T)
            processingPayloadFormat.setDataType(DataType_Int8);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_8U)
            processingPayloadFormat.setDataType(DataType_UInt8);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_16T)
            processingPayloadFormat.setDataType(DataType_Int16);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_16U)
            processingPayloadFormat.setDataType(DataType_UInt16);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_32F)
            processingPayloadFormat.setDataType(DataType_Float);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_32T)
            processingPayloadFormat.setDataType(DataType_Int32);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_32U)
            processingPayloadFormat.setDataType(DataType_UInt32);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_64F)
            processingPayloadFormat.setDataType(DataType_Double);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_64T)
            processingPayloadFormat.setDataType(DataType_Int64);
        else if (stream_definition.data_format.data_item_format == BULKIO::VITA49_64U)
            processingPayloadFormat.setDataType(DataType_UInt64);

        processingPayloadFormat.setRepeating(stream_definition.data_format.repeating);
        processingPayloadFormat.setEventTagSize((int32_t) stream_definition.data_format.event_tag_size);
        processingPayloadFormat.setChannelTagSize((int32_t) stream_definition.data_format.channel_tag_size);
        processingPayloadFormat.setItemPackingFieldSize((int32_t) stream_definition.data_format.data_item_size);
        processingPayloadFormat.setDataItemSize((int32_t) stream_definition.data_format.data_item_size);
        processingPayloadFormat.setVectorSize((int32_t) stream_definition.data_format.vector_size);
    }

    void SourceVITA49_i::process_context(std::vector<char> *packet) {
        BULKIO::StreamSRI outputSRI;

        outputSRI.blocking = currSRI.blocking;
        outputSRI.hversion = currSRI.hversion;
        outputSRI.mode = currSRI.mode;
        outputSRI.subsize = currSRI.subsize;
        outputSRI.xdelta = currSRI.xdelta;
        outputSRI.xstart = currSRI.xstart;
        outputSRI.xunits = currSRI.xunits;
        outputSRI.ydelta = currSRI.ydelta;
        outputSRI.ystart = currSRI.ystart;
        outputSRI.yunits = currSRI.yunits;

        std::string classID;
        TimeStamp packetTime;
        BULKIO::PrecisionUTCTime T_l;
        T_l.twsec = 0.0;
        T_l.tfsec = 0.0;

        //vector magic!
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer = (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) & (contextPacket_g->bbuf));
        vectorPointer->_M_start = const_cast<char*> ((char*) &((*packet)[_offset]));
        vectorPointer->_M_finish = vectorPointer->_M_start + (packet->size() - _offset);
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
        if (!isNull(contextPacket_g->getStreamID()) && streamID.empty()) {
            streamID = contextPacket_g->getStreamID();
        }

        if (!isNull(contextPacket_g->getClassID())) {
            classID = contextPacket_g->getClassID();
            addModifyKeyword<string>(&outputSRI, "CLASS_IDENTIFIER ", classID);
        }

        if (!isNull(contextPacket_g->getDataPayloadFormat()))
            contextPayloadFormat = contextPacket_g->getDataPayloadFormat();

        if (!isNull(contextPacket_g->getTimeStamp())) {
            //try{
            packetTime = contextPacket_g->getTimeStamp();
            //}catch{

            //}
            if (packetTime.getIntegerMode() == vrt::IntegerMode_UTC || packetTime.getIntegerMode() == vrt::IntegerMode_GPS) {
                T_l.tcstatus = BULKIO::TCS_VALID;
                T_l.twsec = packetTime.getUTCSeconds();
                T_l.tfsec = packetTime.getTimeStampFractional();
                addModifyKeyword<double>(&outputSRI, "TimeStamp Whole Seconds ", CORBA::Double(T_l.twsec));
                addModifyKeyword<double>(&outputSRI, "TimeStamp Fractional Seconds ", CORBA::Double(T_l.tfsec * 1e-12));
                outputSRI.xstart = T_l.twsec + T_l.tfsec * 1e-12;
            } else {
                T_l.tcstatus = BULKIO::TCS_INVALID;
                T_l.twsec = packetTime.getTimeStampInteger();
                T_l.tfsec = packetTime.getTimeStampFractional();
                addModifyKeyword<double>(&outputSRI, "TimeStamp Whole Seconds ", CORBA::Double(T_l.twsec));
                addModifyKeyword<double>(&outputSRI, "TimeStamp Fractional Seconds ", CORBA::Double(T_l.tfsec));
            }
        }
        if (!isNull(contextPacket_g->getReferencePointIdentifier()) && contextPacket_g->getReferencePointIdentifier() > 0) {
            addModifyKeyword<long>(&outputSRI, "Reference Point Identifier", CORBA::Long(contextPacket_g->getReferencePointIdentifier()));
            LOG_DEBUG(SourceVITA49_i, "Reference Point Identifier: " << contextPacket_g->getReferencePointIdentifier());
        }
        if (!_bulkioPriority) {
            if (!isNull(contextPacket_g->getBandwidth())) {
                addModifyKeyword<double>(&outputSRI, "COL_BW", CORBA::Double(contextPacket_g->getBandwidth()));
                LOG_DEBUG(SourceVITA49_i, "COL_BW: " << contextPacket_g->getBandwidth());
            }
            if (!isNull(contextPacket_g->getFrequencyIF())) {
                addModifyKeyword<double>(&outputSRI, "COL_IF_FREQUENCY", CORBA::Double(contextPacket_g->getFrequencyIF()));
                LOG_DEBUG(SourceVITA49_i, "COL_IF_FREQUENCY: " << contextPacket_g->getFrequencyIF());
            }
            if (!isNull(contextPacket_g->getFrequencyRF())) {
                addModifyKeyword<double>(&outputSRI, "COL_RF", CORBA::Double(contextPacket_g->getFrequencyRF()));
                LOG_DEBUG(SourceVITA49_i, "COL_RF: " << contextPacket_g->getFrequencyRF());
            }
            if (!isNull(contextPacket_g->getFrequencyOffsetRF())) {
                addModifyKeyword<double>(&outputSRI, "COL_RF_OFFSET", CORBA::Double(contextPacket_g->getFrequencyOffsetRF()));
                LOG_DEBUG(SourceVITA49_i, "COL_RF_OFFSET: " << contextPacket_g->getFrequencyOffsetRF());
            }
            if (!isNull(contextPacket_g->getBandOffsetIF())) {
                addModifyKeyword<double>(&outputSRI, "COL_IF_FREQUENCY_OFFSET", CORBA::Double(contextPacket_g->getBandOffsetIF()));
                LOG_DEBUG(SourceVITA49_i, "COL_IF_FREQUENCY_OFFSET: " << contextPacket_g->getBandOffsetIF());
            }
            if (!isNull(contextPacket_g->getReferenceLevel())) {
                addModifyKeyword<float>(&outputSRI, "COL_REFERENCE_LEVEL", CORBA::Float(contextPacket_g->getReferenceLevel()));
                LOG_DEBUG(SourceVITA49_i, "COL_REFERENCE_LEVEL: " << contextPacket_g->getReferenceLevel());
            }
            float total = 0;
            if (!isNull(contextPacket_g->getGain1())) {
                addModifyKeyword<float>(&outputSRI, "COL_GAIN", CORBA::Float(contextPacket_g->getGain1()));
                total += contextPacket_g->getGain1();
                LOG_DEBUG(SourceVITA49_i, "COL_GAIN: " << contextPacket_g->getGain1());

            }
            if (!isNull(contextPacket_g->getGain2())) {
                total += contextPacket_g->getGain2();
                addModifyKeyword<float>(&outputSRI, "DATA_GAIN", CORBA::Float(contextPacket_g->getGain2()));
                if (total != 0) {
                    addModifyKeyword<float>(&outputSRI, "ATTENUATION_SUM", CORBA::Float(total));
                    LOG_DEBUG(SourceVITA49_i, "ATTENUATION_SUM:" << total);
                }
                LOG_DEBUG(SourceVITA49_i, "DATA_GAIN:" << contextPacket_g->getGain2());
            }
            if (!isNull(contextPacket_g->getOverRangeCount())) {
                addModifyKeyword<int64_t>(&outputSRI, "OVER_RANGE_SUM", CORBA::Long(contextPacket_g->getOverRangeCount()));
                LOG_DEBUG(SourceVITA49_i, "OVER_RANGE_SUM: " << contextPacket_g->getOverRangeCount());
            }
            if (!isNull(contextPacket_g->getSampleRate())) {
                outputSRI.xdelta = 1.0 / contextPacket_g->getSampleRate();
                connection_status.input_sample_rate = 1.0 / contextPacket_g->getSampleRate();
                inputSampleRate = contextPacket_g->getSampleRate();
                LOG_DEBUG(SourceVITA49_i, "Sample Rate: " << contextPacket_g->getSampleRate());
            }
        }
        if (!isNull(contextPacket_g->getTimeStampAdjustment())) {
            addModifyKeyword<int64_t>(&outputSRI, "TIMESTAMP_ADJUSTMENT", CORBA::Long(contextPacket_g->getTimeStampAdjustment()));
            LOG_DEBUG(SourceVITA49_i, "TIMESTAMP_ADJUSTMENT_PICOSECOND: " << contextPacket_g->getTimeStampAdjustment());
        }
        if (!isNull(contextPacket_g->getTimeStampCalibration())) {
            addModifyKeyword<long>(&outputSRI, "TIMESTAMP_CALIBRATION", CORBA::Long(contextPacket_g->getTimeStampCalibration()));
            LOG_DEBUG(SourceVITA49_i, "TIMESTAMP_CALIBRATION: " << contextPacket_g->getTimeStampCalibration());
        }
        if (!isNull(contextPacket_g->getTemperature())) {
            addModifyKeyword<float>(&outputSRI, "TEMPERATURE", CORBA::Float(contextPacket_g->getTemperature()));
            LOG_DEBUG(SourceVITA49_i, "TEMPERATURE: " << contextPacket_g->getTemperature());
        }
        if (!isNull(contextPacket_g->getDeviceIdentifier())) {
            addModifyKeyword<string>(&outputSRI, "DEVICE_IDENTIFIER", CORBA::string_dup(contextPacket_g->getDeviceID().c_str()));
            LOG_DEBUG(SourceVITA49_i, "DEVICE_IDENTIFIER: " << contextPacket_g->getDeviceIdentifier());
        }


        if (!isNull(contextPacket_g->isCalibratedTimeStamp()) && contextPacket_g->isCalibratedTimeStamp() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "CALIBRATED_TIME_STAMP", true);
            LOG_DEBUG(SourceVITA49_i, "CALIBRATED_TIME_STAMP: " << true);
        }

        if (!isNull(contextPacket_g->isDataValid()) && contextPacket_g->isDataValid() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "DATA_VALID", true);
            LOG_DEBUG(SourceVITA49_i, "DATA_VALID: " << true);
        }

        if (!isNull(contextPacket_g->isReferenceLocked()) && contextPacket_g->isReferenceLocked() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "REFERENCE_LOCKED", true);
            LOG_DEBUG(SourceVITA49_i, "REFERENCE_LOCKED: " << true);
        }

        if (!isNull(contextPacket_g->isAutomaticGainControl()) && contextPacket_g->isAutomaticGainControl() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "AUTO_GAIN_CONTROL", true);
            LOG_DEBUG(SourceVITA49_i, "AUTO_GAIN_CONTROL: " << true);
        }
        if (!isNull(contextPacket_g->isSignalDetected()) && contextPacket_g->isSignalDetected() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "SIGNAL_DETECTION", true);
            LOG_DEBUG(SourceVITA49_i, "SIGNAL_DETECTION: " << true);
        }
        if (!isNull(contextPacket_g->isInvertedSpectrum()) && contextPacket_g->isInvertedSpectrum() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "DATA_INVERSION", true);
            LOG_DEBUG(SourceVITA49_i, "DATA_INVERSION: " << true);
        }
        if (!isNull(contextPacket_g->isOverRange()) && contextPacket_g->isOverRange() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "OVER_RANGE", true);
            LOG_DEBUG(SourceVITA49_i, "OVER_RANGE: " << true);
        }
        if (!isNull(contextPacket_g->isDiscontinuous()) && contextPacket_g->isDiscontinuous() == _TRUE) {
            addModifyKeyword<bool>(&outputSRI, "SAMPLE_LOSS", true);
            LOG_DEBUG(SourceVITA49_i, "SAMPLE_LOSS: " << true);
        }
        //this is a warning.....
        if (contextPacket_g->getUserDefinedBits() != 0) {
            addModifyKeyword<long>(&outputSRI, " USER_DEFINED ", CORBA::Long(contextPacket_g->getUserDefinedBits()));
            LOG_DEBUG(SourceVITA49_i, "USER_DEFINED: " << contextPacket_g->getUserDefinedBits());
        }

        //if class id is not defined than lets get the data type
        if (processingPayloadFormat != contextPacket_g->getDataPayloadFormat() && !isNull(contextPacket_g->getDataPayloadFormat())) {
            processingPayloadFormat = contextPacket_g->getDataPayloadFormat();
        }
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        //std::cout << !_bulkioPriority << std::endl;

        //if (!_bulkioPriority) {
        //    if (processingPayloadFormat.getRealComplexType() == RealComplexType_ComplexCartesian) {
        //        //std::cout << "Complex Mode " << std::endl;
        //        outputSRI.mode = 1;
        //    } else {
        //        outputSRI.mode = 0;
        //    }
        //}
        //std::cout << outputSRI.mode << std::endl;
        if (!isNull(contextPacket_g->getGeolocationGPS())) {
            LOG_DEBUG(SourceVITA49_i, " The GEO GPS info exists");
            Geolocation Temp = contextPacket_g->getGeolocationGPS();
            if (processingGeolocation != Temp) {
                processingGeolocation = contextPacket_g->getGeolocationGPS();

                TimeStamp geo_gps_time;
                geo_gps_time = processingGeolocation.getTimeStamp();

                if (!isNull(geo_gps_time.getIntegerMode())) {
                    geolocation_gps_structure.TIME_SECONDS = geo_gps_time.getSecondsGPS();
                    geolocation_gps_structure.TIME_FRACTIONAL = CORBA::Long(geo_gps_time.getPicoSeconds());
                } else {
                    geolocation_gps_structure.TIME_SECONDS = 0;
                    geolocation_gps_structure.TIME_FRACTIONAL = 0;
                }

                if (processingGeolocation.getManufacturerIdentifier() != INT32_NULL)
                    geolocation_gps_structure.MANUFACTURER_ID = processingGeolocation.getManufacturerIdentifier();
                else
                    geolocation_gps_structure.MANUFACTURER_ID = 0;

                if (processingGeolocation.getLatitude() != vrt::DOUBLE_NAN)
                    geolocation_gps_structure.LATITUDE = processingGeolocation.getLatitude();
                else
                    geolocation_gps_structure.LATITUDE = 0.0;

                if (processingGeolocation.getLongitude() != DOUBLE_NAN)
                    geolocation_gps_structure.LONGITUDE = processingGeolocation.getLongitude();
                else
                    geolocation_gps_structure.LONGITUDE = 0.0;

                if (processingGeolocation.getAltitude() != DOUBLE_NAN)
                    geolocation_gps_structure.ALTITUDE = processingGeolocation.getAltitude();
                else
                    geolocation_gps_structure.ALTITUDE = 0.0;

                if (processingGeolocation.getSpeedOverGround() != DOUBLE_NAN)
                    geolocation_gps_structure.GROUND_SPEED = processingGeolocation.getSpeedOverGround();
                else
                    geolocation_gps_structure.GROUND_SPEED = 0.0;

                if (processingGeolocation.getHeadingAngle() != DOUBLE_NAN)
                    geolocation_gps_structure.HEADING_ANGLE = processingGeolocation.getHeadingAngle();
                else
                    geolocation_gps_structure.HEADING_ANGLE = 0.0;

                if (processingGeolocation.getTrackAngle() != DOUBLE_NAN)
                    geolocation_gps_structure.TRACK_ANGLE = processingGeolocation.getTrackAngle();
                else
                    geolocation_gps_structure.TRACK_ANGLE = 0.0;

                if (processingGeolocation.getMagneticVariation() != DOUBLE_NAN)
                    geolocation_gps_structure.MAGNETIC_VARIATION = processingGeolocation.getMagneticVariation();
                else
                    geolocation_gps_structure.MAGNETIC_VARIATION = 0.0;

                addModifyKeyword<GEOLOCATION_GPS_struct>(&outputSRI, "GEOLOCATION_GPS", geolocation_gps_structure);
            }
        }

        if (!isNull(contextPacket_g->getGeolocationINS())) {
            LOG_DEBUG(SourceVITA49_i, " The GEO INS info exists");
            Geolocation Temp = contextPacket_g->getGeolocationINS();

            if (processingGEOINS != Temp) {
                processingGEOINS = contextPacket_g->getGeolocationGPS();

                TimeStamp geo_ins_time;
                geo_ins_time = processingGEOINS.getTimeStamp();

                if (!isNull(geo_ins_time.getIntegerMode())) {
                    geolocation_ins_structure.TIME_SECONDS = geo_ins_time.getSecondsGPS();
                    geolocation_ins_structure.TIME_FRACTIONAL = CORBA::Long(geo_ins_time.getPicoSeconds());
                } else {
                    geolocation_ins_structure.TIME_SECONDS = 0;
                    geolocation_ins_structure.TIME_FRACTIONAL = 0;
                }

                if (processingGEOINS.getManufacturerIdentifier() != INT32_NULL)
                    geolocation_ins_structure.MANUFACTURER_ID = processingGEOINS.getManufacturerIdentifier();
                else
                    geolocation_ins_structure.MANUFACTURER_ID = 0;

                if (processingGEOINS.getLatitude() != vrt::DOUBLE_NAN)
                    geolocation_ins_structure.LATITUDE = processingGEOINS.getLatitude();
                else
                    geolocation_ins_structure.LATITUDE = 0.0;

                if (processingGEOINS.getLongitude() != DOUBLE_NAN)
                    geolocation_ins_structure.LONGITUDE = processingGEOINS.getLongitude();
                else
                    geolocation_ins_structure.LONGITUDE = 0.0;

                if (processingGEOINS.getAltitude() != DOUBLE_NAN)
                    geolocation_ins_structure.ALTITUDE = processingGEOINS.getAltitude();
                else
                    geolocation_ins_structure.ALTITUDE = 0.0;

                if (processingGEOINS.getSpeedOverGround() != DOUBLE_NAN)
                    geolocation_ins_structure.GROUND_SPEED = processingGEOINS.getSpeedOverGround();
                else
                    geolocation_ins_structure.GROUND_SPEED = 0.0;

                if (processingGEOINS.getHeadingAngle() != DOUBLE_NAN)
                    geolocation_ins_structure.HEADING_ANGLE = processingGEOINS.getHeadingAngle();
                else
                    geolocation_ins_structure.HEADING_ANGLE = 0.0;

                if (processingGEOINS.getTrackAngle() != DOUBLE_NAN)
                    geolocation_ins_structure.TRACK_ANGLE = processingGEOINS.getTrackAngle();
                else
                    geolocation_ins_structure.TRACK_ANGLE = 0.0;

                if (processingGEOINS.getMagneticVariation() != DOUBLE_NAN)
                    geolocation_ins_structure.MAGNETIC_VARIATION = processingGEOINS.getMagneticVariation();
                else
                    geolocation_ins_structure.MAGNETIC_VARIATION = 0.0;

                addModifyKeyword<GEOLOCATION_INS_struct>(&outputSRI, "GEOLOCATION_INS", geolocation_ins_structure);
            }
        }
        if (!isNull(contextPacket_g->getEphemerisECEF())) {
            LOG_DEBUG(SourceVITA49_i, " The Ephemeris ECEF info exists");
            Ephemeris Temp = contextPacket_g->getEphemerisECEF();
            if (!isNull(Temp.getAdjunct())) {
                EphemerisAdjunct Adjunct = Temp.getAdjunct();
            }
            if (processingEphemeris != Temp) {

                TimeStamp eph_ecef_time;
                eph_ecef_time = processingEphemeris.getTimeStamp();

                if (!isNull(eph_ecef_time.getIntegerMode())) {
                    ephemeris_ecef_structure.TIME_SECONDS = eph_ecef_time.getSecondsGPS();
                    ephemeris_ecef_structure.TIME_FRACTIONAL_SECONDS = CORBA::Long(eph_ecef_time.getPicoSeconds());
                } else {
                    ephemeris_ecef_structure.TIME_SECONDS = 0;
                    ephemeris_ecef_structure.TIME_FRACTIONAL_SECONDS = 0;
                }

                if (processingEphemeris.getPositionX() != vrt::DOUBLE_NAN)
                    ephemeris_ecef_structure.POSITION_X = processingEphemeris.getPositionX();
                else
                    ephemeris_ecef_structure.POSITION_X = 0.0;

                if (processingEphemeris.getPositionY() != DOUBLE_NAN)
                    ephemeris_ecef_structure.POSITION_Y = processingEphemeris.getPositionY();
                else
                    ephemeris_ecef_structure.POSITION_Y = 0.0;

                if (processingEphemeris.getPositionZ() != DOUBLE_NAN)
                    ephemeris_ecef_structure.POSITION_Z = processingEphemeris.getPositionZ();
                else
                    ephemeris_ecef_structure.POSITION_Z = 0.0;

                if (processingEphemeris.getAttitudeAlpha() != DOUBLE_NAN)
                    ephemeris_ecef_structure.ATTITUDE_ALPHA = processingEphemeris.getAttitudeAlpha();
                else
                    ephemeris_ecef_structure.ATTITUDE_ALPHA = 0.0;

                if (processingEphemeris.getAttitudeBeta() != DOUBLE_NAN)
                    ephemeris_ecef_structure.ATTITUDE_BETA = processingEphemeris.getAttitudeBeta();
                else
                    ephemeris_ecef_structure.ATTITUDE_BETA = 0.0;

                if (processingEphemeris.getAttitudePhi() != DOUBLE_NAN)
                    ephemeris_ecef_structure.ATTITUDE_PHI = processingEphemeris.getAttitudePhi();
                else
                    ephemeris_ecef_structure.ATTITUDE_PHI = 0.0;

                if (processingEphemeris.getVelocityX() != DOUBLE_NAN)
                    ephemeris_ecef_structure.VELOCITY_X = processingEphemeris.getVelocityX();
                else
                    ephemeris_ecef_structure.VELOCITY_X = 0.0;

                if (processingEphemeris.getVelocityY() != DOUBLE_NAN)
                    ephemeris_ecef_structure.VELOCITY_Y = processingEphemeris.getVelocityY();
                else
                    ephemeris_ecef_structure.VELOCITY_Y = 0.0;

                if (processingEphemeris.getVelocityZ() != DOUBLE_NAN)
                    ephemeris_ecef_structure.VELOCITY_Z = processingEphemeris.getVelocityZ();
                else
                    ephemeris_ecef_structure.VELOCITY_Z = 0.0;
                //			KNOWN BUG - ADJUNCT EPHEMERIS CLASS DOES NOT WORK AS EXPECTED
                //			if (processingEphemeris.getRotationalVelocityAlpha() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ROTATIONAL_VELOCITY_ALPHA = processingEphemeris.getRotationalVelocityAlpha();
                //			else
                //				ephemeris_ecef_structure.ROTATIONAL_VELOCITY_ALPHA = 0.0;
                //
                //			if (processingEphemeris.getRotationalVelocityBeta() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ROTATIONAL_VELOCITY_BETA = processingEphemeris.getRotationalVelocityBeta();
                //			else
                //				ephemeris_ecef_structure.ROTATIONAL_VELOCITY_BETA = 0.0;
                //
                //			if (processingEphemeris.getRotationalVelocityPhi() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ROTATIONAL_VELOCITY_PHI = processingEphemeris.getRotationalVelocityPhi();
                //			else
                //				ephemeris_ecef_structure.ROTATIONAL_VELOCITY_PHI = 0.0;
                //
                //			if (processingEphemeris.getAccelerationX() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ACCELERATION_X = processingEphemeris.getAccelerationX();
                //			else
                //				ephemeris_ecef_structure.ACCELERATION_X = 0.0;
                //
                //			if (processingEphemeris.getAccelerationY() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ACCELERATION_Y = processingEphemeris.getAccelerationY();
                //			else
                //				ephemeris_ecef_structure.ACCELERATION_Y = 0.0;
                //
                //			if (processingEphemeris.getAccelerationZ() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ACCELERATION_Z = processingEphemeris.getAccelerationZ();
                //			else
                //				ephemeris_ecef_structure.ACCELERATION_Z = 0.0;
                //
                //			if (processingEphemeris.getRotationalAccelerationAlpha() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ROTATIONAL_ACCELERATION_ALPHA = processingEphemeris.getRotationalAccelerationAlpha();
                //			else
                //				ephemeris_ecef_structure.ROTATIONAL_ACCELERATION_ALPHA = 0.0;
                //
                //			if (processingEphemeris.getRotationalAccelerationBeta() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ROTATIONAL_ACCELERATION_BETA = processingEphemeris.getRotationalAccelerationBeta();
                //			else
                //				ephemeris_ecef_structure.ROTATIONAL_ACCELERATION_BETA = 0.0;
                //
                //			if (processingEphemeris.getRotationalAccelerationPhi() != DOUBLE_NAN)
                //				ephemeris_ecef_structure.ROTATIONAL_ACCELERATION_PHI = processingEphemeris.getRotationalAccelerationPhi();
                //			else
                //				ephemeris_ecef_structure.ROTATIONAL_ACCELERATION_PHI = 0.0;

                addModifyKeyword<EPHEMERIS_ECEF_struct>(&outputSRI, "EPHEMERIS_ECEF", ephemeris_ecef_structure);
            }
        }

        if (!isNull(contextPacket_g->getEphemerisRelative())) {
            LOG_DEBUG(SourceVITA49_i, " The Ephemeris Relative info exists");
            Ephemeris Temp = contextPacket_g->getEphemerisRelative();

            if (processingEphemerisRel != Temp) {
                processingEphemerisRel = contextPacket_g->getEphemerisRelative();

                TimeStamp eph_rel_time;
                eph_rel_time = processingEphemerisRel.getTimeStamp();

                if (!isNull(eph_rel_time.getIntegerMode())) {
                    ephemeris_relative_structure.TIME_SECONDS = eph_rel_time.getSecondsGPS();
                    ephemeris_relative_structure.TIME_FRACTIONAL_SECONDS = CORBA::Long(eph_rel_time.getPicoSeconds());
                } else {
                    ephemeris_relative_structure.TIME_SECONDS = 0;
                    ephemeris_relative_structure.TIME_FRACTIONAL_SECONDS = 0;
                }

                if (processingEphemerisRel.getPositionX() != vrt::DOUBLE_NAN)
                    ephemeris_relative_structure.POSITION_X = processingEphemerisRel.getPositionX();
                else
                    ephemeris_relative_structure.POSITION_X = 0.0;

                if (processingEphemerisRel.getPositionY() != DOUBLE_NAN)
                    ephemeris_relative_structure.POSITION_Y = processingEphemerisRel.getPositionY();
                else
                    ephemeris_relative_structure.POSITION_Y = 0.0;

                if (processingEphemerisRel.getPositionZ() != DOUBLE_NAN)
                    ephemeris_relative_structure.POSITION_Z = processingEphemerisRel.getPositionZ();
                else
                    ephemeris_relative_structure.POSITION_Z = 0.0;

                if (processingEphemerisRel.getAttitudeAlpha() != DOUBLE_NAN)
                    ephemeris_relative_structure.ATTITUDE_ALPHA = processingEphemerisRel.getAttitudeAlpha();
                else
                    ephemeris_relative_structure.ATTITUDE_ALPHA = 0.0;

                if (processingEphemerisRel.getAttitudeBeta() != DOUBLE_NAN)
                    ephemeris_relative_structure.ATTITUDE_BETA = processingEphemerisRel.getAttitudeBeta();
                else
                    ephemeris_relative_structure.ATTITUDE_BETA = 0.0;

                if (processingEphemerisRel.getAttitudePhi() != DOUBLE_NAN)
                    ephemeris_relative_structure.ATTITUDE_PHI = processingEphemerisRel.getAttitudePhi();
                else
                    ephemeris_relative_structure.ATTITUDE_PHI = 0.0;

                if (processingEphemerisRel.getVelocityX() != DOUBLE_NAN)
                    ephemeris_relative_structure.VELOCITY_X = processingEphemerisRel.getVelocityX();
                else
                    ephemeris_relative_structure.VELOCITY_X = 0.0;

                if (processingEphemerisRel.getVelocityY() != DOUBLE_NAN)
                    ephemeris_relative_structure.VELOCITY_Y = processingEphemerisRel.getVelocityY();
                else
                    ephemeris_relative_structure.VELOCITY_Y = 0.0;

                if (processingEphemerisRel.getVelocityZ() != DOUBLE_NAN)
                    ephemeris_relative_structure.VELOCITY_Z = processingEphemerisRel.getVelocityZ();
                else
                    ephemeris_relative_structure.VELOCITY_Z = 0.0;
                //			KNOWN BUG - ADJUNCT EPHEMERIS CLASS DOES NOT WORK AS EXPECTED
                //			if (processingEphemerisRel.getRotationalVelocityAlpha() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ROTATIONAL_VELOCITY_ALPHA = processingEphemerisRel.getRotationalVelocityAlpha();
                //			else
                //				ephemeris_relative_structure.ROTATIONAL_VELOCITY_ALPHA = 0.0;
                //
                //			if (processingEphemerisRel.getRotationalVelocityBeta() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ROTATIONAL_VELOCITY_BETA = processingEphemerisRel.getRotationalVelocityBeta();
                //			else
                //				ephemeris_relative_structure.ROTATIONAL_VELOCITY_BETA = 0.0;
                //
                //			if (processingEphemerisRel.getRotationalVelocityPhi() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ROTATIONAL_VELOCITY_PHI = processingEphemerisRel.getRotationalVelocityPhi();
                //			else
                //				ephemeris_relative_structure.ROTATIONAL_VELOCITY_PHI = 0.0;
                //
                //			if (processingEphemerisRel.getAccelerationX() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ACCELERATION_X = processingEphemerisRel.getAccelerationX();
                //			else
                //				ephemeris_relative_structure.ACCELERATION_X = 0.0;
                //
                //			if (processingEphemerisRel.getAccelerationY() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ACCELERATION_Y = processingEphemerisRel.getAccelerationY();
                //			else
                //				ephemeris_relative_structure.ACCELERATION_Y = 0.0;
                //
                //			if (processingEphemerisRel.getAccelerationZ() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ACCELERATION_Z = processingEphemerisRel.getAccelerationZ();
                //			else
                //				ephemeris_relative_structure.ACCELERATION_Z = 0.0;
                //
                //			if (processingEphemerisRel.getRotationalAccelerationAlpha() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ROTATIONAL_ACCELERATION_ALPHA = processingEphemerisRel.getRotationalAccelerationAlpha();
                //			else
                //				ephemeris_relative_structure.ROTATIONAL_ACCELERATION_ALPHA = 0.0;
                //
                //			if (processingEphemerisRel.getRotationalAccelerationBeta() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ROTATIONAL_ACCELERATION_BETA = processingEphemerisRel.getRotationalAccelerationBeta();
                //			else
                //				ephemeris_relative_structure.ROTATIONAL_ACCELERATION_BETA = 0.0;
                //
                //			if (processingEphemerisRel.getRotationalAccelerationPhi() != DOUBLE_NAN)
                //				ephemeris_relative_structure.ROTATIONAL_ACCELERATION_PHI = processingEphemerisRel.getRotationalAccelerationPhi();
                //			else
                //				ephemeris_relative_structure.ROTATIONAL_ACCELERATION_PHI = 0.0;

                addModifyKeyword<EPHEMERIS_RELATIVE_struct>(&outputSRI, "EPHEMERIS_RELATIVE", ephemeris_relative_structure);
            }
        }


        //currently no support for ephemeris reference identifer format or geosentences
        //std::string temp = (contextPacket_g->getGeoSentences()).getSentences();
        //addModifyKeyword<string>(&outputSRI, " GEO_SENTENCES ", CORBA::string_dup(temp.c_str()));
        /*
                std::vector<int32_t> SourceContext = (contextPacket_g->getContextAssocLists()).getSourceContext();
                if (SourceContext.size() != 0){
                        for (unsigned int i = 0; i<SourceContext.size();++i){
                                std::string value = " SOURCE_CONTEXT.";
                                value += boost::lexical_cast<std::string>(i);
                                addModifyKeyword<int32_t>(&outputSRI,CORBA::string_dup(value.c_str()), SourceContext[i]);

                        }
                }
                std::vector<int32_t> SystemContext = (contextPacket_g->getContextAssocLists()).getSystemContext();
                if (SourceContext.size() != 0){
                        for (unsigned int i = 0; i<SystemContext.size();++i){
                                std::string value = " SYSTEM_CONTEXT.";
                                value += boost::lexical_cast<std::string>(i);
                                addModifyKeyword<int32_t>(&outputSRI,CORBA::string_dup(value.c_str()), SystemContext[i]);

                        }
                }
                std::vector<int32_t> VectorComponent = (contextPacket_g->getContextAssocLists()).getVectorComponent();
                if (SourceContext.size() != 0){
                        for (unsigned int i = 0; i<VectorComponent.size();++i){
                                std::string value = " SYSTEM_CONTEXT.";
                                value += boost::lexical_cast<std::string>(i);
                                addModifyKeyword<int32_t>(&outputSRI,CORBA::string_dup(value.c_str()), VectorComponent[i]);

                        }
                }
                std::vector<int32_t> AsynchronousChannel = (contextPacket_g->getContextAssocLists()).getAsynchronousChannel();
                if (SourceContext.size() != 0){
                        for (unsigned int i = 0; i<AsynchronousChannel.size();++i){
                                std::string value = " ASYNCHRONOUS_CHANNEL.";
                                value += boost::lexical_cast<std::string>(i);
                                addModifyKeyword<int32_t>(&outputSRI,CORBA::string_dup(value.c_str()), AsynchronousChannel[i]);
                        }
                }
                std::vector<int32_t> AsynchronousChannelTag = (contextPacket_g->getContextAssocLists()).getAsynchronousChannelTag();
                if (SourceContext.size() != 0){
                        for (unsigned int i = 0; i<AsynchronousChannelTag.size();++i){
                                std::string value = " ASYNCHRONOUS_CHANNEL_TAG.";
                                value += boost::lexical_cast<std::string>(i);
                                addModifyKeyword<int32_t>(&outputSRI,CORBA::string_dup(value.c_str()), AsynchronousChannelTag[i]);
                        }
                }*/
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        receivedContextPacket = true;
        mergeRecSRI(outputSRI, false);

    }

    bool SourceVITA49_i::process_data_packet(std::vector<char> *packet) {
        TimeStamp packetTime_s;

        if (!canProcessDataPacket()) {
            return false;
        }

        // If we're ready to process, we can calculate and validate result
        number_of_bytes = processingPayloadFormat.getDataItemSize() / 8;
        if (number_of_bytes <= 0) {
            LOG_WARN(SourceVITA49_i, "Unable to decode received data packet: Malformed payload format - Dropping data packet!")
            return false;
        }

        boost::mutex::scoped_lock runLock(processing_lock);
        _readIndex = 0;
        //vector magic
        std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *vectorPointer = (std::_Vector_base<char, _seqVector::seqVectorAllocator<char> >::_Vector_impl *) ((void*) & (standardDPacket->bbuf));
        vectorPointer->_M_start = const_cast<char*> ((char*) &((*packet)[_offset]));
        vectorPointer->_M_finish = vectorPointer->_M_start + (packet->size() - _offset);
        vectorPointer->_M_end_of_storage = vectorPointer->_M_finish;
        //if we have dropped a packet or this is the first time through....
        if (!init) {
            if (dataPacketCount != standardDPacket->getPacketCount()) {
                droppedPacket = true;
                connection_status.packets_missing++;
                droppedPacket = standardDPacket->getPacketCount();
                LOG_ERROR(SourceVITA49_i, "Error: SourceVITA49 :: Dropped packet :: Packet Count expected " << dataPacketCount << " actual packet Count " << standardDPacket->getPacketCount());
                dataPacketCount = standardDPacket->getPacketCount();
                if (processingPayloadFormat.getDataType() == DataType_Int8) {
                    dataChar_out->pushPacket((char*) data, _writeIndex, T, false, streamID);
                } else if (processingPayloadFormat.getDataType() == DataType_UInt8) {
                    dataOctet_out->pushPacket((unsigned char*) data, _writeIndex, T, false, streamID);
                } else if (processingPayloadFormat.getDataType() == DataType_Int16) {
                    dataShort_out->pushPacket((short*) data, _writeIndex / 2, T, false, streamID);
                } else if (processingPayloadFormat.getDataType() == DataType_UInt16) {
                    dataUshort_out->pushPacket((unsigned short*) data, _writeIndex / 2, T, false, streamID);
                } else if (processingPayloadFormat.getDataType() == DataType_Float) {
                    dataFloat_out->pushPacket((float*) data, _writeIndex / 4, T, false, streamID);
                }else if(processingPayloadFormat.getDataType() == DataType_Double){
                    dataDouble_out->pushPacket((double*) data, _writeIndex / 6, T, false, streamID);
                }
                _writeIndex = 0;
                getTimeStamp = true;
                samplesSinceLastTimeStamp = 0;
            }
        } else { //otherwise keep processing as is
            init = false;
            droppedPacket = false;
        }

        //calculate what the next received packet index should be.
        dataPacketCount++;
        if (dataPacketCount == 16)
            dataPacketCount = 0;

        if (getTimeStamp) {
            if (!isNull(standardDPacket->getTimeStamp())) {
                packetTime_s = standardDPacket->getTimeStamp();
                if ((packetTime_s.getIntegerMode() == 2) || (packetTime_s.getIntegerMode() == 1)) {
                    if (samplesSinceLastTimeStamp > 0)
                        T = adjustTime(packetTime_s, true);
                    else
                        T = adjustTime(packetTime_s, false);
                }
            }
        }


        int packetLength = standardDPacket->getPayloadLength(); //(in bytes)
        //if (packetLenght > 0){
        int needed = 0;
        int available = 0;
        int length = 0;
        if (_dataRef != BYTE_ORDER) {
            standardDPacket->swapPayloadBytes(processingPayloadFormat, &array[0]);
        }
        //std::cout << " readIndex is " << _readIndex << " writeIndex is " << _writeIndex << std::endl;
        //std::cout << "transferSize is " << transferSize << std::endl;
        //std::cout << "number_of_bytes " << number_of_bytes << std::endl;
        needed = transferSize - _writeIndex;
        available = packetLength - _readIndex;
        //std::cout << "available is " << available << " packet length is "<<packetLength<<std::endl;std::cout.flush();
        while (available > 0 && curr_attach.attach) {
            //std::cout << "available: " << available << std::endl;
            if (available >= needed) {
                length = needed;
                memcpy(&data[_writeIndex], standardDPacket->getData_normal(processingPayloadFormat, _readIndex), length);
                _readIndex = _readIndex + length;
                _writeIndex = 0;
                needed = transferSize;
            } else if (available > 0) {// Not enough data in input buffer
                length = available;
                memcpy(&data[_writeIndex], standardDPacket->getData_normal(processingPayloadFormat, _readIndex), length);
                _readIndex += length;
                _writeIndex += length;
            }

            if (available >= length) {
                available -= length;
                //std::cout << " !!!!!!!!! "<<number_of_bytes << std::endl;std::cout.flush();
                samplesSinceLastTimeStamp += (length / number_of_bytes) / (1 * currSRI.mode + 1);

                if (_writeIndex == 0) {
                    if (processingPayloadFormat.getDataType() == DataType_Int8) {
                        dataChar_out->pushPacket((char*) data, transferSize, T, false, streamID);
                    } else if (processingPayloadFormat.getDataType() == DataType_UInt8) {
                        dataOctet_out->pushPacket((unsigned char*) data, transferSize, T, false, streamID);
                    } else if (processingPayloadFormat.getDataType() == DataType_Int16) {
                        dataShort_out->pushPacket((short*) data, transferSize / 2,  T, false, streamID);
                    } else if (processingPayloadFormat.getDataType() == DataType_UInt16) {
                        dataUshort_out->pushPacket((unsigned short*) data, transferSize / 2, T, false, streamID);
                    } else if (processingPayloadFormat.getDataType() == DataType_Float) {
                        dataFloat_out->pushPacket((float*) data, transferSize / 4, T, false, streamID);
                    } else if (processingPayloadFormat.getDataType() == DataType_Double) {
                        dataDouble_out->pushPacket((double*) data, transferSize / 8, T, false, streamID);
                    }
                    //update data_throughput

                    connection_status.data_throughput = transferSize / (timeDiff());

                    getTimeStamp = true;

                    if ((unsigned int) available >= transferSize) {
                        //adjust time stamp to first sample after push
                        getTimeStamp = true;
                        T = adjustTime(packetTime_s, false);
                    }//else
                    samplesSinceLastTimeStamp = 0;

                }
            }
        }
        return true;
    }

    /******************************************************************************************
     * mergeRecSRI()
     *
     * Takes:   BULKIO::StreamSRI&
     * Returns: void
     *
     * Functionality:
     *
     ******************************************************************************************/

    void SourceVITA49_i::mergeRecSRI(const BULKIO::StreamSRI &recSRI, bool force_refresh) {
        bool receivedNewSRI = force_refresh;
        boost::mutex::scoped_lock lock(sriLock);
        //std::cout << __PRETTY_FUNCTION__ << std::endl;
        //printSRI(&recSRI, "INPUT SRI");
        if (!compareSRI(recSRI, currSRI)) {
            receivedNewSRI = true;
            currSRI.hversion = recSRI.hversion;
            currSRI.xstart = recSRI.xstart;
            currSRI.xunits = recSRI.xunits;
            std::string temp_streamID = ossie::corba::returnString(recSRI.streamID);
            if (!temp_streamID.empty())
                currSRI.streamID = recSRI.streamID;

            if (streamID.empty())
                streamID = currSRI.streamID;
            /* # frames to be delivered by pushPacket() call; set to 0 for single packet */
            currSRI.ystart = recSRI.ystart;
            currSRI.ydelta = recSRI.ydelta;
            currSRI.yunits = recSRI.yunits;
            currSRI.mode = recSRI.mode;
            currSRI.xdelta = recSRI.xdelta;

            //search through currSRI for the keywords in recSRI
            //this should be the first case....
            unsigned long keySize = recSRI.keywords.length();
            unsigned long currSize = currSRI.keywords.length();

            for (unsigned long i = 0; i < keySize; ++i) {
                //want to search all of currSize
                std::string action = "eq";
                //look for the id and set to true of found
                bool foundID = false;
                for (unsigned long j = 0; j < currSize; ++j) {
                    if (strcmp(recSRI.keywords[i].id, currSRI.keywords[j].id) == 0) {
                        foundID = true;
                        if (!ossie::compare_anys(recSRI.keywords[i].value, currSRI.keywords[j].value, action)) {

                            currSRI.keywords[j].value = recSRI.keywords[i].value;
                        }
                    }

                }
                //the id was not found we need to extend the length of keywords in currSRI and add this one to the end
                if (!foundID) {

                    unsigned long keySize_t = currSRI.keywords.length();
                    currSRI.keywords.length(keySize_t + 1);
                    currSRI.keywords[keySize_t].id = CORBA::string_dup(recSRI.keywords[i].id);
                    currSRI.keywords[keySize_t].value = recSRI.keywords[i].value;
                }
                if (string(recSRI.keywords[i].id) == "dataRef" || string(recSRI.keywords[i].id) == "DATA_REF_STR") {

                    CORBA::Long value;
                    recSRI.keywords[i].value >>= value;
                    std::string value2;
                    _dataRef = (uint32_t) atol(ossie::any_to_string(recSRI.keywords[i].value).c_str());
                }
            }
        }

        if (receivedNewSRI) {
            if (dataChar_out->isActive())
                dataChar_out->pushSRI(currSRI);
            if (dataOctet_out->isActive())
                dataOctet_out->pushSRI(currSRI);
            if (dataShort_out->isActive())
                dataShort_out->pushSRI(currSRI);
            if (dataUshort_out->isActive()) 
                dataUshort_out->pushSRI(currSRI);
            if (dataFloat_out->isActive())
                dataFloat_out->pushSRI(currSRI);
            if (dataDouble_out->isActive())
                dataDouble_out->pushSRI(currSRI);
        }
    }

    /****************************************************************************************
     * compareSRI()
     *
     * Takes:   BULKIO::StreamSRI&, BULKIO::StreamSRI&
     * Returns: bool
     *
     * Functionality:
     *    Compares the two SRIs passed in, returns true if they are equal, false otherwise
     ****************************************************************************************/
    bool SourceVITA49_i::compareSRI(BULKIO::StreamSRI A, BULKIO::StreamSRI B) {
        bool same = false;

        if ((A.hversion == B.hversion) and
                (A.xstart == B.xstart) and
                (A.xunits == B.xunits) and
                (A.ystart == B.ystart) and
                (A.ydelta == B.ydelta) and

                /* these values aren't checked because we get them from the   packet headers */
                //(A.mode == B.mode) and
                //(A.xdelta == B.xdelta) and
                //(A.subsize == B.subsize) and

                (!strcmp(A.streamID, B.streamID))) {
            same = true;
        } else {
            same = false;
            return same;
        }

        // THIS DOESN"T WORK RIGHT... ONCE THE CONTEXT PACKET IS PARSED,
        // MORE SRI KEYWORDS ARE ADDED SO SRI RECEIVED ON THE DATAVITA PORT
        // WILL BE A SUBSET OF THE CURRENT SRI
        //if (A.keywords.length() != B.keywords.length()) {
        //    same = false;
        //    std::cout << "Diff keyword lengths" << std::endl;
        //    return same;
        //}

        for (unsigned int i = 0; i < A.keywords.length(); i++) {
            std::string action = "ne";
            if (ossie::compare_anys(A.keywords[i].value, B.keywords[i].value, action)) {
                same = false;
                return same;
            }
        }
        return same;
    }

    bool SourceVITA49_i::isStreamDefinitionValid() {
        return streamDefinition.valid_data_format;
    }

    /***********************************************************************************************
     * clear
     * ()
     *
     * Takes:   BULKIO:: StreamDefinition, std::string
     * Returns: std::string
     *
     * Functionality:
     *    Extra functionality for the attach function which is called by the bulkio:data  port
     *    inside of port_impl.cpp.  This function pulls the relevant information out of the
     *     StreamDefinition to open a connection to the multicast.  The streamID is returned
     ***********************************************************************************************/
    char* SourceVITA49_i::attach(const BULKIO::VITA49StreamDefinition& stream, const char* userid)
             throw (BULKIO::dataVITA49::AttachError, BULKIO::dataVITA49::StreamInputError) 
    {
        printStreamDef(stream);
        boost::mutex::scoped_lock runLock(running_lock);

        // Can't do this.  Breaks down when:
        //    1.) No Connection/Attachment Exists
        //    2.) Manual override enabled
        //    3.) Connection is now made
        //    4.) Manual override disabled 
        //if (curr_attach.manual_override) {
        //    LOG_ERROR(SourceVITA49_i, "Not accepting external connections")
        //    //throw BULKIO::dataVITA49::AttachError("SourceVITA49 is currently in override mode and is not accepting external connections");
        //}
        
        if ( !attach_port_settings.attach_id.empty() ) {
            LOG_ERROR(SourceVITA49_i, "Already has an attached connection! Latest attachment ignored!")
            //throw BULKIO::dataVITA49::AttachError("SourceVITA49 already has a connectioned stream.");
        }
               
        initialize_values();
        boost::mutex::scoped_lock lock(sriLock);
        
        // Save off the streamDefinition
        streamDefinition = stream;
        
        // Save off the received attach settings
        attach_port_settings.ip_address = stream.ip_address;
        attach_port_settings.vlan = stream.vlan;
        attach_port_settings.port = stream.port;
        attach_port_settings.manual_override = false;
        attach_port_settings.attach_id = UUID_HELPER::new_uuid();
        //curr_attach.attach_id = attach_port_settings.attach_id;

        if (streamDefinition.protocol == BULKIO::VITA49_UDP_TRANSPORT)
            attach_port_settings.use_udp_protocol = true;
        else
            attach_port_settings.use_udp_protocol = false;

        // Update the SRI
        if (isStreamDefinitionValid()) {
            if (processingPayloadFormat.getRealComplexType() == RealComplexType_ComplexCartesian) {
                currSRI.mode = 1;
            } else {
                currSRI.mode = 0;
            }
        }
        streamID = std::string(stream.id);
        currSRI.streamID = CORBA::string_dup(streamID.c_str());
        
        lock.unlock();

        // Destroy the rx_thread to allow new settings to take effect
        destroy_rx_thread();

        return CORBA::string_dup(attach_port_settings.attach_id.c_str());
    }

    bool SourceVITA49_i::canProcessDataPacket() {
        boost::mutex::scoped_lock runLock(processing_lock);
        
        if (VITA49Processing_override.enable && receivedValidSRI)
            initPayloadFormat(overrideStreamDefinition);
        else if (receivedContextPacket)
            processingPayloadFormat = contextPayloadFormat;
        else if (isStreamDefinitionValid() && receivedValidSRI) {
            initPayloadFormat(streamDefinition);
        } else
            return false;

        return true;
    }
    
    void SourceVITA49_i::resetPayloadFormat() {
        receivedValidSRI = false;
        receivedContextPacket = false;
        resetStreamDefinition(contextStreamDefinition);
    }

    void SourceVITA49_i::resetStreamDefinition(BULKIO::VITA49StreamDefinition& streamDef) {
        streamDef.data_format.packing_method_processing_efficient = true;
        streamDef.data_format.complexity = BULKIO::VITA49_COMPLEX_CARTESIAN;
        streamDef.data_format.data_item_format = BULKIO::VITA49_32F;
        streamDef.data_format.repeating = 1;
        streamDef.data_format.event_tag_size = 0;
        streamDef.data_format.channel_tag_size = 32;
        streamDef.data_format.item_packing_field_size = 32;
        streamDef.data_format.data_item_size = 1;
        streamDef.data_format.vector_size = 0;
        streamDef.ip_address = "127.0.0.1";
        streamDef.port = 0;
        streamDef.vlan = 0;
        streamDef.protocol = BULKIO::VITA49_UDP_TRANSPORT;
    }
    /***********************************************************************************************
     * detach()
     *
     * Takes:   std::string
     * Returns: void
     *
     * Functionality:
     *    Extra functionality for the detach function which is called by the bulkio:data  port
     *    inside of port_impl.cpp
     ***********************************************************************************************/
    void SourceVITA49_i::detach(const char* attach_id) {
        std::string attachId(attach_id);

        boost::mutex::scoped_lock runLock(running_lock);
        LOG_TRACE( SourceVITA49_i, "Detach " );
        if (attachId != attach_port_settings.attach_id) {
            LOG_WARN(SourceVITA49_i, "ATTACHMENT ID (STREAM ID) NOT FOUND FOR: " << attachId);
            throw BULKIO::dataVITA49::DetachError("Detach called on stream not currently running");
        }

        destroy_rx_thread();
        if (processingPayloadFormat.getDataType() == DataType_Int8) {
            dataChar_out->pushPacket((char*) data, _writeIndex, T, true, streamID);
        } else if (processingPayloadFormat.getDataType() == DataType_UInt8) {
            dataOctet_out->pushPacket((unsigned char*) data, _writeIndex, T, true, streamID);
        } else if (processingPayloadFormat.getDataType() == DataType_Int16) {
            dataShort_out->pushPacket((short*) data, _writeIndex / 2, T, true, streamID);
        } else if (processingPayloadFormat.getDataType() == DataType_UInt16) {
            dataUshort_out->pushPacket((unsigned short*) data, _writeIndex / 2, T, true, streamID);
        } else if (processingPayloadFormat.getDataType() == DataType_Float) {
            dataFloat_out->pushPacket((float*) data, _writeIndex / 4, T, true, streamID);
        } else if (processingPayloadFormat.getDataType() == DataType_Double) {
            dataDouble_out->pushPacket((double*) data, _writeIndex / 8, T, true, streamID);
        }
        
        resetAttachSettings(attach_port_settings);
        
        //clear the buffers and reset the memory
        streamID = "";
        connection_status.input_enabled = false;
        createMem = true;
        initialize_values();
        boost::mutex::scoped_lock lock(sriLock);
    }

    /****************************************************************************************
     * setStartOfYear()
     *
     * Takes:   void
     * Returns: void
     *
     * Functionality:
     *    Calculates the number of seconds that have passed from the EPOCH (Jan 1 1970) to
     *    00:00 Jan 1 of the current year.  This value is used to build the PrecisionUTC
     *    Time Tag from the time tag in the   packets
     ****************************************************************************************/
    void SourceVITA49_i::setStartOfYear() {
        time_t systemtime;
        tm *systemtime_struct;

        time(&systemtime);
        /* System Time in a struct of day, month, year */
        systemtime_struct = localtime(&systemtime);
        /* Find time from EPOCH to Jan 1st of current year */
        systemtime_struct->tm_sec = 0;
        systemtime_struct->tm_min = 0;
        systemtime_struct->tm_hour = 0;
        systemtime_struct->tm_mday = 1;
        systemtime_struct->tm_mon = 0;
        startofyear = mktime(systemtime_struct);
    }
    
    void SourceVITA49_i::printStreamDef(const BULKIO::VITA49StreamDefinition& streamDef) {
        std::string space = "    ";
        LOG_DEBUG(SourceVITA49_i, " *** STREAM DEFINITION *** ");
        LOG_DEBUG(SourceVITA49_i, space << "ip_address: " << streamDef.ip_address);
        LOG_DEBUG(SourceVITA49_i, space << "vlan: " << streamDef.vlan);
        LOG_DEBUG(SourceVITA49_i, space << "port: " << streamDef.port);
        LOG_DEBUG(SourceVITA49_i, space << "protocol: " << streamDef.protocol);
        LOG_DEBUG(SourceVITA49_i, space << "valid_data_format: " << streamDef.valid_data_format);
        LOG_DEBUG(SourceVITA49_i, space << "data_format: ");
        LOG_DEBUG(SourceVITA49_i, space << space << "packing_method_processing_efficient: " << streamDef.data_format.packing_method_processing_efficient);
        LOG_DEBUG(SourceVITA49_i, space << space << "complexity: " << streamDef.data_format.complexity);
        LOG_DEBUG(SourceVITA49_i, space << space << "data_item_format: " << streamDef.data_format.data_item_format);
        LOG_DEBUG(SourceVITA49_i, space << space << "repeating: " << streamDef.data_format.repeating);
        LOG_DEBUG(SourceVITA49_i, space << space << "event_tag_size: " << streamDef.data_format.event_tag_size);
        LOG_DEBUG(SourceVITA49_i, space << space << "channel_tag_size: " << streamDef.data_format.channel_tag_size);
        LOG_DEBUG(SourceVITA49_i, space << space << "item_packing_field_size: " << streamDef.data_format.item_packing_field_size);
        LOG_DEBUG(SourceVITA49_i, space << space << "data_item_size: " << streamDef.data_format.data_item_size);
        LOG_DEBUG(SourceVITA49_i, space << space << "repeat_count: " << streamDef.data_format.repeat_count);
        LOG_DEBUG(SourceVITA49_i, space << space << "vector_size: " << streamDef.data_format.vector_size);
        LOG_DEBUG(SourceVITA49_i, "");
    };

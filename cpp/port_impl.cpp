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
 */

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

 	Source: port_impl.cpp
 	Generated on: Fri Jan 10 11:20:07 EST 2014
 	REDHAWK IDE
 	Version: 1.8.6
 	Build id: N201312130030

*******************************************************************************************/

#include "SourceVITA49.h"
// ----------------------------------------------------------------------------------------
// BULKIO_dataUshort_Out_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataUshort_Out_i::BULKIO_dataUshort_Out_i(std::string port_name, SourceVITA49_base *_parent) :
Port_Uses_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    recConnectionsRefresh = false;
    recConnections.length(0);
}

BULKIO_dataUshort_Out_i::~BULKIO_dataUshort_Out_i()
{
}

/*
 * pushSRI
 *     description: send out SRI describing the data payload
 *
 *  H: structure of type BULKIO::StreamSRI with the SRI for this stream
 *    hversion
 *    xstart: start time of the stream
 *    xdelta: delta between two samples
 *    xunits: unit types from Platinum specification
 *    subsize: 0 if the data is one-dimensional
 *    ystart
 *    ydelta
 *    yunits: unit types from Platinum specification
 *    mode: 0-scalar, 1-complex
 *    streamID: stream identifier
 *    sequence<CF::DataType> keywords: unconstrained sequence of key-value pairs for additional description
 */
void BULKIO_dataUshort_Out_i::pushSRI(const BULKIO::StreamSRI& H)
{
    std::vector < std::pair < BULKIO::dataUshort_var, std::string > >::iterator i;

    boost::mutex::scoped_lock lock(updatingPortsLock);   // don't want to process while command information is coming in

    if (active) {
        for (i = outConnections.begin(); i != outConnections.end(); ++i) {
            try {
                ((*i).first)->pushSRI(H);
            } catch(...) {
                std::cout << "Call to pushSRI by BULKIO_dataUshort_Out_i failed" << std::endl;
            }
        }
    }

    currentSRIs[std::string(H.streamID)] = H;
    refreshSRI = false;

    return;
}



// ----------------------------------------------------------------------------------------
// BULKIO_dataShort_Out_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataShort_Out_i::BULKIO_dataShort_Out_i(std::string port_name, SourceVITA49_base *_parent) :
Port_Uses_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    recConnectionsRefresh = false;
    recConnections.length(0);
}

BULKIO_dataShort_Out_i::~BULKIO_dataShort_Out_i()
{
}

/*
 * pushSRI
 *     description: send out SRI describing the data payload
 *
 *  H: structure of type BULKIO::StreamSRI with the SRI for this stream
 *    hversion
 *    xstart: start time of the stream
 *    xdelta: delta between two samples
 *    xunits: unit types from Platinum specification
 *    subsize: 0 if the data is one-dimensional
 *    ystart
 *    ydelta
 *    yunits: unit types from Platinum specification
 *    mode: 0-scalar, 1-complex
 *    streamID: stream identifier
 *    sequence<CF::DataType> keywords: unconstrained sequence of key-value pairs for additional description
 */
void BULKIO_dataShort_Out_i::pushSRI(const BULKIO::StreamSRI& H)
{
    std::vector < std::pair < BULKIO::dataShort_var, std::string > >::iterator i;

    boost::mutex::scoped_lock lock(updatingPortsLock);   // don't want to process while command information is coming in

    if (active) {
        for (i = outConnections.begin(); i != outConnections.end(); ++i) {
            try {
                ((*i).first)->pushSRI(H);
            } catch(...) {
                std::cout << "Call to pushSRI by BULKIO_dataShort_Out_i failed" << std::endl;
            }
        }
    }

    currentSRIs[std::string(H.streamID)] = H;
    refreshSRI = false;

    return;
}



// ----------------------------------------------------------------------------------------
// BULKIO_dataChar_Out_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataChar_Out_i::BULKIO_dataChar_Out_i(std::string port_name, SourceVITA49_base *_parent) :
Port_Uses_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    recConnectionsRefresh = false;
    recConnections.length(0);
}

BULKIO_dataChar_Out_i::~BULKIO_dataChar_Out_i()
{
}

/*
 * pushSRI
 *     description: send out SRI describing the data payload
 *
 *  H: structure of type BULKIO::StreamSRI with the SRI for this stream
 *    hversion
 *    xstart: start time of the stream
 *    xdelta: delta between two samples
 *    xunits: unit types from Platinum specification
 *    subsize: 0 if the data is one-dimensional
 *    ystart
 *    ydelta
 *    yunits: unit types from Platinum specification
 *    mode: 0-scalar, 1-complex
 *    streamID: stream identifier
 *    sequence<CF::DataType> keywords: unconstrained sequence of key-value pairs for additional description
 */
void BULKIO_dataChar_Out_i::pushSRI(const BULKIO::StreamSRI& H)
{
    std::vector < std::pair < BULKIO::dataChar_var, std::string > >::iterator i;

    boost::mutex::scoped_lock lock(updatingPortsLock);   // don't want to process while command information is coming in

    if (active) {
        for (i = outConnections.begin(); i != outConnections.end(); ++i) {
            try {
                ((*i).first)->pushSRI(H);
            } catch(...) {
                std::cout << "Call to pushSRI by BULKIO_dataChar_Out_i failed" << std::endl;
            }
        }
    }

    currentSRIs[std::string(H.streamID)] = H;
    refreshSRI = false;

    return;
}



// ----------------------------------------------------------------------------------------
// BULKIO_dataDouble_Out_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataDouble_Out_i::BULKIO_dataDouble_Out_i(std::string port_name, SourceVITA49_base *_parent) :
Port_Uses_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    recConnectionsRefresh = false;
    recConnections.length(0);
}

BULKIO_dataDouble_Out_i::~BULKIO_dataDouble_Out_i()
{
}

/*
 * pushSRI
 *     description: send out SRI describing the data payload
 *
 *  H: structure of type BULKIO::StreamSRI with the SRI for this stream
 *    hversion
 *    xstart: start time of the stream
 *    xdelta: delta between two samples
 *    xunits: unit types from Platinum specification
 *    subsize: 0 if the data is one-dimensional
 *    ystart
 *    ydelta
 *    yunits: unit types from Platinum specification
 *    mode: 0-scalar, 1-complex
 *    streamID: stream identifier
 *    sequence<CF::DataType> keywords: unconstrained sequence of key-value pairs for additional description
 */
void BULKIO_dataDouble_Out_i::pushSRI(const BULKIO::StreamSRI& H)
{
    std::vector < std::pair < BULKIO::dataDouble_var, std::string > >::iterator i;

    boost::mutex::scoped_lock lock(updatingPortsLock);   // don't want to process while command information is coming in

    if (active) {
        for (i = outConnections.begin(); i != outConnections.end(); ++i) {
            try {
                ((*i).first)->pushSRI(H);
            } catch(...) {
                std::cout << "Call to pushSRI by BULKIO_dataDouble_Out_i failed" << std::endl;
            }
        }
    }

    currentSRIs[std::string(H.streamID)] = H;
    refreshSRI = false;

    return;
}



// ----------------------------------------------------------------------------------------
// BULKIO_dataFloat_Out_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataFloat_Out_i::BULKIO_dataFloat_Out_i(std::string port_name, SourceVITA49_base *_parent) :
Port_Uses_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    recConnectionsRefresh = false;
    recConnections.length(0);
}

BULKIO_dataFloat_Out_i::~BULKIO_dataFloat_Out_i()
{
}

/*
 * pushSRI
 *     description: send out SRI describing the data payload
 *
 *  H: structure of type BULKIO::StreamSRI with the SRI for this stream
 *    hversion
 *    xstart: start time of the stream
 *    xdelta: delta between two samples
 *    xunits: unit types from Platinum specification
 *    subsize: 0 if the data is one-dimensional
 *    ystart
 *    ydelta
 *    yunits: unit types from Platinum specification
 *    mode: 0-scalar, 1-complex
 *    streamID: stream identifier
 *    sequence<CF::DataType> keywords: unconstrained sequence of key-value pairs for additional description
 */
void BULKIO_dataFloat_Out_i::pushSRI(const BULKIO::StreamSRI& H)
{
    std::vector < std::pair < BULKIO::dataFloat_var, std::string > >::iterator i;

    boost::mutex::scoped_lock lock(updatingPortsLock);   // don't want to process while command information is coming in

    if (active) {
        for (i = outConnections.begin(); i != outConnections.end(); ++i) {
            try {
                ((*i).first)->pushSRI(H);
            } catch(...) {
                std::cout << "Call to pushSRI by BULKIO_dataFloat_Out_i failed" << std::endl;
            }
        }
    }

    currentSRIs[std::string(H.streamID)] = H;
    refreshSRI = false;

    return;
}



// ----------------------------------------------------------------------------------------
// BULKIO_dataOctet_Out_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataOctet_Out_i::BULKIO_dataOctet_Out_i(std::string port_name, SourceVITA49_base *_parent) :
Port_Uses_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    recConnectionsRefresh = false;
    recConnections.length(0);
}

BULKIO_dataOctet_Out_i::~BULKIO_dataOctet_Out_i()
{
}

/*
 * pushSRI
 *     description: send out SRI describing the data payload
 *
 *  H: structure of type BULKIO::StreamSRI with the SRI for this stream
 *    hversion
 *    xstart: start time of the stream
 *    xdelta: delta between two samples
 *    xunits: unit types from Platinum specification
 *    subsize: 0 if the data is one-dimensional
 *    ystart
 *    ydelta
 *    yunits: unit types from Platinum specification
 *    mode: 0-scalar, 1-complex
 *    streamID: stream identifier
 *    sequence<CF::DataType> keywords: unconstrained sequence of key-value pairs for additional description
 */
void BULKIO_dataOctet_Out_i::pushSRI(const BULKIO::StreamSRI& H)
{
    std::vector < std::pair < BULKIO::dataOctet_var, std::string > >::iterator i;

    boost::mutex::scoped_lock lock(updatingPortsLock);   // don't want to process while command information is coming in

    if (active) {
        for (i = outConnections.begin(); i != outConnections.end(); ++i) {
            try {
                ((*i).first)->pushSRI(H);
            } catch(...) {
                std::cout << "Call to pushSRI by BULKIO_dataOctet_Out_i failed" << std::endl;
            }
        }
    }

    currentSRIs[std::string(H.streamID)] = H;
    refreshSRI = false;

    return;
}


// ----------------------------------------------------------------------------------------
// BULKIO_dataVITA49_In_i definition
// ----------------------------------------------------------------------------------------
BULKIO_dataVITA49_In_i::BULKIO_dataVITA49_In_i(std::string port_name, SourceVITA49_base *_parent) : Port_Provides_base_impl(port_name)
{
    parent = static_cast<SourceVITA49_i *> (_parent);
    sriChanged = false;
}

BULKIO_dataVITA49_In_i::~BULKIO_dataVITA49_In_i()
{
}

BULKIO::PortStatistics * BULKIO_dataVITA49_In_i::statistics()
{
    boost::mutex::scoped_lock lock(statUpdateLock);
    BULKIO::PortStatistics_var recStat = new BULKIO::PortStatistics(stats.retrieve());
    return recStat._retn();
}

BULKIO::PortUsageType BULKIO_dataVITA49_In_i::state()
{
    if (attachedStreamMap.size() == 0) {
        return BULKIO::IDLE;
    } else if (attachedStreamMap.size() == 1) {
        return BULKIO::BUSY;
    } else {
        return BULKIO::ACTIVE;
    }
}

BULKIO::StreamSRISequence * BULKIO_dataVITA49_In_i::attachedSRIs()
{
    boost::mutex::scoped_lock lock(sriUpdateLock);
    BULKIO::StreamSRISequence_var sris = new BULKIO::StreamSRISequence();
    sris->length(currentHs.size());
    std::map<std::string, std::pair<BULKIO::StreamSRI, BULKIO::PrecisionUTCTime> >::iterator sriIter;
    unsigned int idx = 0;

    sriIter = currentHs.begin();
    while (sriIter != currentHs.end()) {
        sris[idx++] = (*sriIter).second.first;
        sriIter++;
    }
    return sris._retn();
}

void BULKIO_dataVITA49_In_i::pushSRI(const BULKIO::StreamSRI& H, const BULKIO::PrecisionUTCTime& T)
{
	boost::mutex::scoped_lock lock(sriUpdateLock);
    bool foundSRI = false;
    BULKIO::StreamSRI tmpH = H;
    std::map<std::string, std::pair<BULKIO::StreamSRI, BULKIO::PrecisionUTCTime> >::iterator sriIter;

    sriIter = currentHs.begin();
    while (sriIter != currentHs.end()) {
        if (strcmp(H.streamID, (*sriIter).first.c_str()) == 0) {
            foundSRI = true;
            break;
        }
        sriIter++;
    }

    if (!foundSRI) {
        currentHs.insert(std::make_pair(CORBA::string_dup(H.streamID), std::make_pair(H, T)));
    } else {
        (*sriIter).second = std::make_pair(H, T);
    }
    sriChanged = true;
}

BULKIO::VITA49StreamDefinition* BULKIO_dataVITA49_In_i::getStreamDefinition(const char* attachId)
{
    std::map<std::string, BULKIO::VITA49StreamDefinition*>::iterator portIter2;
    portIter2 = attachedStreamMap.begin();
    // use: attachedPorts[(*portIter).first] :instead
    while (portIter2 != attachedStreamMap.end()) {
        if (strcmp((*portIter2).first.c_str(), attachId) == 0) {
            return (*portIter2).second;
        }
        portIter2++;
    }
    return NULL;
}

char* BULKIO_dataVITA49_In_i::getUser(const char* attachId)
{
    std::map<std::string, std::string>::iterator portIter2;
    portIter2 = attachedUsers.begin();
    while (portIter2 != attachedUsers.end()) {
        if (strcmp((*portIter2).first.c_str(), attachId) == 0) {
            return CORBA::string_dup((*portIter2).second.c_str());
        }
        portIter2++;
    }
    return NULL;
}

BULKIO::dataVITA49::InputUsageState BULKIO_dataVITA49_In_i::usageState()
{
    if (attachedStreamMap.size() == 0) {
        return BULKIO::dataVITA49::IDLE;
    } else if (attachedStreamMap.size() == 1) {
        return BULKIO::dataVITA49::BUSY;
    } else {
        return BULKIO::dataVITA49::ACTIVE;
    }
}

BULKIO::VITA49StreamSequence* BULKIO_dataVITA49_In_i::attachedStreams()
{
    BULKIO::VITA49StreamSequence* seq = new BULKIO::VITA49StreamSequence();
    seq->length(attachedStreamMap.size());
    std::map<std::string, BULKIO::VITA49StreamDefinition*>::iterator portIter2;
    portIter2 = attachedStreamMap.begin();
    unsigned int i = 0;
    while (portIter2 != attachedStreamMap.end()) {
        (*seq)[i++] = *((*portIter2).second);
        portIter2++;
    }
    return seq;
}

BULKIO::StringSequence* BULKIO_dataVITA49_In_i::attachmentIds()
{
    BULKIO::StringSequence* seq = new BULKIO::StringSequence();
    seq->length(attachedStreamMap.size());
    std::map<std::string, BULKIO::VITA49StreamDefinition*>::iterator portIter2;
    portIter2 = attachedStreamMap.begin();
    unsigned int i = 0;
    while (portIter2 != attachedStreamMap.end()) {
        (*seq)[i++] = CORBA::string_dup((*portIter2).first.c_str());
        portIter2++;
    }
    return seq;
}

char* BULKIO_dataVITA49_In_i::attach(const BULKIO::VITA49StreamDefinition& stream, const char* userid) throw (BULKIO::dataVITA49::AttachError, BULKIO::dataVITA49::StreamInputError)
{
    std::string attachId;

    attachId = parent->attach(stream, userid);
    attachedStreamMap.insert(std::make_pair(attachId, new BULKIO::VITA49StreamDefinition(stream)));
    attachedUsers.insert(std::make_pair(attachId, std::string(userid)));

    return CORBA::string_dup(attachId.c_str());
}

void BULKIO_dataVITA49_In_i::detach(const char* attachId)
{
    parent->detach(attachId);
    attachedStreamMap.erase(attachId);
    attachedUsers.erase(attachId);
}



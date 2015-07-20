/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of REDHAWK SourceVITA49.
 *
 * REDHAWK SourceVITA49 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * REDHAWK SourceVITA49 is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#ifndef STRUCTPROPS_H
#define STRUCTPROPS_H

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

*******************************************************************************************/

#include <ossie/CorbaUtils.h>

struct attachment_override_struct {
    attachment_override_struct ()
    {
        enabled = false;
        ip_address = "127.0.0.1";
        vlan = 0;
        port = 12344;
        use_udp_protocol = true;
    };

    static std::string getId() {
        return std::string("attachment_override");
    };

    bool enabled;
    std::string ip_address;
    unsigned short vlan;
    CORBA::Long port;
    bool use_udp_protocol;
};

inline bool operator>>= (const CORBA::Any& a, attachment_override_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("attachment_override::enabled", props[idx].id)) {
            if (!(props[idx].value >>= s.enabled)) return false;
        }
        else if (!strcmp("attachment_override::ip_address", props[idx].id)) {
            if (!(props[idx].value >>= s.ip_address)) return false;
        }
        else if (!strcmp("attachment_override::vlan", props[idx].id)) {
            if (!(props[idx].value >>= s.vlan)) return false;
        }
        else if (!strcmp("attachment_override::port", props[idx].id)) {
            if (!(props[idx].value >>= s.port)) return false;
        }
        else if (!strcmp("transportMethod_override::use_udp_protocol", props[idx].id)) {
            if (!(props[idx].value >>= s.use_udp_protocol)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const attachment_override_struct& s) {
    CF::Properties props;
    props.length(5);
    props[0].id = CORBA::string_dup("attachment_override::enabled");
    props[0].value <<= s.enabled;
    props[1].id = CORBA::string_dup("attachment_override::ip_address");
    props[1].value <<= s.ip_address;
    props[2].id = CORBA::string_dup("attachment_override::vlan");
    props[2].value <<= s.vlan;
    props[3].id = CORBA::string_dup("attachment_override::port");
    props[3].value <<= s.port;
    props[4].id = CORBA::string_dup("transportMethod_override::use_udp_protocol");
    props[4].value <<= s.use_udp_protocol;
    a <<= props;
};

inline bool operator== (const attachment_override_struct& s1, const attachment_override_struct& s2) {
    if (s1.enabled!=s2.enabled)
        return false;
    if (s1.ip_address!=s2.ip_address)
        return false;
    if (s1.vlan!=s2.vlan)
        return false;
    if (s1.port!=s2.port)
        return false;
    if (s1.use_udp_protocol!=s2.use_udp_protocol)
        return false;
    return true;
};

inline bool operator!= (const attachment_override_struct& s1, const attachment_override_struct& s2) {
    return !(s1==s2);
};

struct connection_status_struct {
    connection_status_struct ()
    {
    };

    static std::string getId() {
        return std::string("connection_status");
    };

    bool input_enabled;
    std::string input_ip_address;
    unsigned short input_port;
    unsigned short input_vlan;
    CORBA::ULong packets_missing;
    double input_sample_rate;
    double data_throughput;
    bool waiting_for_context_packet;
};

inline bool operator>>= (const CORBA::Any& a, connection_status_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("connection_status::input_enabled", props[idx].id)) {
            if (!(props[idx].value >>= s.input_enabled)) return false;
        }
        else if (!strcmp("connection_status::input_ip_address", props[idx].id)) {
            if (!(props[idx].value >>= s.input_ip_address)) return false;
        }
        else if (!strcmp("connection_status::input_port", props[idx].id)) {
            if (!(props[idx].value >>= s.input_port)) return false;
        }
        else if (!strcmp("connection_status::input_vlan", props[idx].id)) {
            if (!(props[idx].value >>= s.input_vlan)) return false;
        }
        else if (!strcmp("connection_status::packets_missing", props[idx].id)) {
            if (!(props[idx].value >>= s.packets_missing)) return false;
        }
        else if (!strcmp("connection_status::input_sample_rate", props[idx].id)) {
            if (!(props[idx].value >>= s.input_sample_rate)) return false;
        }
        else if (!strcmp("connection_status::data_throughput", props[idx].id)) {
            if (!(props[idx].value >>= s.data_throughput)) return false;
        }
        else if (!strcmp("connection_status::waiting_for_context_packet", props[idx].id)) {
            if (!(props[idx].value >>= s.waiting_for_context_packet)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const connection_status_struct& s) {
    CF::Properties props;
    props.length(8);
    props[0].id = CORBA::string_dup("connection_status::input_enabled");
    props[0].value <<= s.input_enabled;
    props[1].id = CORBA::string_dup("connection_status::input_ip_address");
    props[1].value <<= s.input_ip_address;
    props[2].id = CORBA::string_dup("connection_status::input_port");
    props[2].value <<= s.input_port;
    props[3].id = CORBA::string_dup("connection_status::input_vlan");
    props[3].value <<= s.input_vlan;
    props[4].id = CORBA::string_dup("connection_status::packets_missing");
    props[4].value <<= s.packets_missing;
    props[5].id = CORBA::string_dup("connection_status::input_sample_rate");
    props[5].value <<= s.input_sample_rate;
    props[6].id = CORBA::string_dup("connection_status::data_throughput");
    props[6].value <<= s.data_throughput;
    props[7].id = CORBA::string_dup("connection_status::waiting_for_context_packet");
    props[7].value <<= s.waiting_for_context_packet;
    a <<= props;
};

inline bool operator== (const connection_status_struct& s1, const connection_status_struct& s2) {
    if (s1.input_enabled!=s2.input_enabled)
        return false;
    if (s1.input_ip_address!=s2.input_ip_address)
        return false;
    if (s1.input_port!=s2.input_port)
        return false;
    if (s1.input_vlan!=s2.input_vlan)
        return false;
    if (s1.packets_missing!=s2.packets_missing)
        return false;
    if (s1.input_sample_rate!=s2.input_sample_rate)
        return false;
    if (s1.data_throughput!=s2.data_throughput)
        return false;
    if (s1.waiting_for_context_packet!=s2.waiting_for_context_packet)
        return false;
    return true;
};

inline bool operator!= (const connection_status_struct& s1, const connection_status_struct& s2) {
    return !(s1==s2);
};

struct VITA49Processing_override_struct {
    VITA49Processing_override_struct ()
    {
        enable = false;
        VRL_frames = false;
        processing_efficient = true;
        real_complex_type = 1;
        data_item_format = 3LL;
        repeating = 1;
        channel_tag_size = 0;
        event_tag_size = 0;
        vector_size = 1;
    };

    static std::string getId() {
        return std::string("VITA49Processing_override");
    };

    bool enable;
    bool VRL_frames;
    bool processing_efficient;
    CORBA::Long real_complex_type;
    CORBA::LongLong data_item_format;
    CORBA::ULong repeating;
    CORBA::ULong channel_tag_size;
    CORBA::ULong event_tag_size;
    CORBA::ULong vector_size;
};

inline bool operator>>= (const CORBA::Any& a, VITA49Processing_override_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("VITA49Processing_override::enable", props[idx].id)) {
            if (!(props[idx].value >>= s.enable)) return false;
        }
        else if (!strcmp("transportMethod_override::VRL_frames", props[idx].id)) {
            if (!(props[idx].value >>= s.VRL_frames)) return false;
        }
        else if (!strcmp("VITA49Processing_override::processing_efficient", props[idx].id)) {
            if (!(props[idx].value >>= s.processing_efficient)) return false;
        }
        else if (!strcmp("VITA49Processing_override::real_complex_type", props[idx].id)) {
            if (!(props[idx].value >>= s.real_complex_type)) return false;
        }
        else if (!strcmp("VITA49Processing_override::data_item_format", props[idx].id)) {
            if (!(props[idx].value >>= s.data_item_format)) return false;
        }
        else if (!strcmp("VITA49Processing_override::repeating", props[idx].id)) {
            if (!(props[idx].value >>= s.repeating)) return false;
        }
        else if (!strcmp("VITA49Processing_override::channel_tag_size", props[idx].id)) {
            if (!(props[idx].value >>= s.channel_tag_size)) return false;
        }
        else if (!strcmp("VITA49Processing_override::event_tag_size", props[idx].id)) {
            if (!(props[idx].value >>= s.event_tag_size)) return false;
        }
        else if (!strcmp("VITA49Processing_override::vector_size", props[idx].id)) {
            if (!(props[idx].value >>= s.vector_size)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const VITA49Processing_override_struct& s) {
    CF::Properties props;
    props.length(9);
    props[0].id = CORBA::string_dup("VITA49Processing_override::enable");
    props[0].value <<= s.enable;
    props[1].id = CORBA::string_dup("transportMethod_override::VRL_frames");
    props[1].value <<= s.VRL_frames;
    props[2].id = CORBA::string_dup("VITA49Processing_override::processing_efficient");
    props[2].value <<= s.processing_efficient;
    props[3].id = CORBA::string_dup("VITA49Processing_override::real_complex_type");
    props[3].value <<= s.real_complex_type;
    props[4].id = CORBA::string_dup("VITA49Processing_override::data_item_format");
    props[4].value <<= s.data_item_format;
    props[5].id = CORBA::string_dup("VITA49Processing_override::repeating");
    props[5].value <<= s.repeating;
    props[6].id = CORBA::string_dup("VITA49Processing_override::channel_tag_size");
    props[6].value <<= s.channel_tag_size;
    props[7].id = CORBA::string_dup("VITA49Processing_override::event_tag_size");
    props[7].value <<= s.event_tag_size;
    props[8].id = CORBA::string_dup("VITA49Processing_override::vector_size");
    props[8].value <<= s.vector_size;
    a <<= props;
};

inline bool operator== (const VITA49Processing_override_struct& s1, const VITA49Processing_override_struct& s2) {
    if (s1.enable!=s2.enable)
        return false;
    if (s1.VRL_frames!=s2.VRL_frames)
        return false;
    if (s1.processing_efficient!=s2.processing_efficient)
        return false;
    if (s1.real_complex_type!=s2.real_complex_type)
        return false;
    if (s1.data_item_format!=s2.data_item_format)
        return false;
    if (s1.repeating!=s2.repeating)
        return false;
    if (s1.channel_tag_size!=s2.channel_tag_size)
        return false;
    if (s1.event_tag_size!=s2.event_tag_size)
        return false;
    if (s1.vector_size!=s2.vector_size)
        return false;
    return true;
};

inline bool operator!= (const VITA49Processing_override_struct& s1, const VITA49Processing_override_struct& s2) {
    return !(s1==s2);
};

struct advanced_configuration_struct {
    advanced_configuration_struct ()
    {
        buffer_size = 4096000;
        corba_transfersize = 0;
        poll_in_time = 100;
        vita49_packet_size = 1500;
    };

    static std::string getId() {
        return std::string("advanced_configuration");
    };

    CORBA::Long buffer_size;
    CORBA::ULong corba_transfersize;
    CORBA::ULong poll_in_time;
    CORBA::ULong vita49_packet_size;
};

inline bool operator>>= (const CORBA::Any& a, advanced_configuration_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("buffer_size", props[idx].id)) {
            if (!(props[idx].value >>= s.buffer_size)) return false;
        }
        else if (!strcmp("corba_transfersize", props[idx].id)) {
            if (!(props[idx].value >>= s.corba_transfersize)) return false;
        }
        else if (!strcmp("poll_in_time", props[idx].id)) {
            if (!(props[idx].value >>= s.poll_in_time)) return false;
        }
        else if (!strcmp("vita49_packet_size", props[idx].id)) {
            if (!(props[idx].value >>= s.vita49_packet_size)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const advanced_configuration_struct& s) {
    CF::Properties props;
    props.length(4);
    props[0].id = CORBA::string_dup("buffer_size");
    props[0].value <<= s.buffer_size;
    props[1].id = CORBA::string_dup("corba_transfersize");
    props[1].value <<= s.corba_transfersize;
    props[2].id = CORBA::string_dup("poll_in_time");
    props[2].value <<= s.poll_in_time;
    props[3].id = CORBA::string_dup("vita49_packet_size");
    props[3].value <<= s.vita49_packet_size;
    a <<= props;
};

inline bool operator== (const advanced_configuration_struct& s1, const advanced_configuration_struct& s2) {
    if (s1.buffer_size!=s2.buffer_size)
        return false;
    if (s1.corba_transfersize!=s2.corba_transfersize)
        return false;
    if (s1.poll_in_time!=s2.poll_in_time)
        return false;
    if (s1.vita49_packet_size!=s2.vita49_packet_size)
        return false;
    return true;
};

inline bool operator!= (const advanced_configuration_struct& s1, const advanced_configuration_struct& s2) {
    return !(s1==s2);
};

#endif // STRUCTPROPS_H

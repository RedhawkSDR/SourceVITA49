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

#ifndef VITA49STRUCTKEYWORDS_H
#define VITA49STRUCTKEYWORDS_H

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

 *******************************************************************************************/

#include <ossie/CorbaUtils.h>
#include <ossie/PropertyInterface.h>

struct GEOLOCATION_GPS_struct {
    GEOLOCATION_GPS_struct ()
    {
        TIME_SECONDS = 0.0;
        TIME_FRACTIONAL = 0.0;
        MANUFACTURER_ID = 0;
        LATITUDE = 0.0;
        LONGITUDE = 0.0;
        ALTITUDE = 0.0;
        GROUND_SPEED = 0.0;
        HEADING_ANGLE = 0.0;
        TRACK_ANGLE = 0.0;
        MAGNETIC_VARIATION = 0.0;
    };

    std::string getId() {
        return std::string("GEOLOCATION_GPS");
    };

    double TIME_SECONDS;
    double TIME_FRACTIONAL;
    CORBA::Long MANUFACTURER_ID;
    double LATITUDE;
    double LONGITUDE;
    double ALTITUDE;
    double GROUND_SPEED;
    double HEADING_ANGLE;
    double TRACK_ANGLE;
    double MAGNETIC_VARIATION;
};

inline bool operator>>= (const CORBA::Any& a, GEOLOCATION_GPS_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("GEOLOCATION_GPS::TIME_SECONDS", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_SECONDS)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::TIME_FRACTIONAL", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_FRACTIONAL)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::MANUFACTURER_ID", props[idx].id)) {
            if (!(props[idx].value >>= s.MANUFACTURER_ID)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::LATITUDE", props[idx].id)) {
            if (!(props[idx].value >>= s.LATITUDE)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::LONGITUDE", props[idx].id)) {
            if (!(props[idx].value >>= s.LONGITUDE)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::ALTITUDE", props[idx].id)) {
            if (!(props[idx].value >>= s.ALTITUDE)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::GROUND_SPEED", props[idx].id)) {
            if (!(props[idx].value >>= s.GROUND_SPEED)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::HEADING_ANGLE", props[idx].id)) {
            if (!(props[idx].value >>= s.HEADING_ANGLE)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::TRACK_ANGLE", props[idx].id)) {
            if (!(props[idx].value >>= s.TRACK_ANGLE)) return false;
        }
        if (!strcmp("GEOLOCATION_GPS::MAGNETIC_VARIATION", props[idx].id)) {
            if (!(props[idx].value >>= s.MAGNETIC_VARIATION)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const GEOLOCATION_GPS_struct& s) {
    CF::Properties props;
    props.length(10);
    props[0].id = CORBA::string_dup("GEOLOCATION_GPS::TIME_SECONDS");
    props[0].value <<= s.TIME_SECONDS;
    props[1].id = CORBA::string_dup("GEOLOCATION_GPS::TIME_FRACTIONAL");
    props[1].value <<= s.TIME_FRACTIONAL;
    props[2].id = CORBA::string_dup("GEOLOCATION_GPS::MANUFACTURER_ID");
    props[2].value <<= s.MANUFACTURER_ID;
    props[3].id = CORBA::string_dup("GEOLOCATION_GPS::LATITUDE");
    props[3].value <<= s.LATITUDE;
    props[4].id = CORBA::string_dup("GEOLOCATION_GPS::LONGITUDE");
    props[4].value <<= s.LONGITUDE;
    props[5].id = CORBA::string_dup("GEOLOCATION_GPS::ALTITUDE");
    props[5].value <<= s.ALTITUDE;
    props[6].id = CORBA::string_dup("GEOLOCATION_GPS::GROUND_SPEED");
    props[6].value <<= s.GROUND_SPEED;
    props[7].id = CORBA::string_dup("GEOLOCATION_GPS::HEADING_ANGLE");
    props[7].value <<= s.HEADING_ANGLE;
    props[8].id = CORBA::string_dup("GEOLOCATION_GPS::TRACK_ANGLE");
    props[8].value <<= s.TRACK_ANGLE;
    props[9].id = CORBA::string_dup("GEOLOCATION_GPS::MAGNETIC_VARIATION");
    props[9].value <<= s.MAGNETIC_VARIATION;
    a <<= props;
};

inline bool operator== (const GEOLOCATION_GPS_struct& s1, const GEOLOCATION_GPS_struct& s2) {
    if (s1.TIME_SECONDS!=s2.TIME_SECONDS)
        return false;
    if (s1.TIME_FRACTIONAL!=s2.TIME_FRACTIONAL)
        return false;
    if (s1.MANUFACTURER_ID!=s2.MANUFACTURER_ID)
        return false;
    if (s1.LATITUDE!=s2.LATITUDE)
        return false;
    if (s1.LONGITUDE!=s2.LONGITUDE)
        return false;
    if (s1.ALTITUDE!=s2.ALTITUDE)
        return false;
    if (s1.GROUND_SPEED!=s2.GROUND_SPEED)
        return false;
    if (s1.HEADING_ANGLE!=s2.HEADING_ANGLE)
        return false;
    if (s1.TRACK_ANGLE!=s2.TRACK_ANGLE)
        return false;
    if (s1.MAGNETIC_VARIATION!=s2.MAGNETIC_VARIATION)
        return false;
    return true;
};

inline bool operator!= (const GEOLOCATION_GPS_struct& s1, const GEOLOCATION_GPS_struct& s2) {
    return !(s1==s2);
};

template<> inline short StructProperty<GEOLOCATION_GPS_struct>::compare (const CORBA::Any& a) {
    if (super::isNil_) {
        if (a.type()->kind() == (CORBA::tk_null)) {
            return 0;
        }
        return 1;
    }

    GEOLOCATION_GPS_struct tmp;
    if (fromAny(a, tmp)) {
        if (tmp != this->value_) {
            return 1;
        }

        return 0;
    } else {
        return 1;
    }
}

struct GEOLOCATION_INS_struct {
    GEOLOCATION_INS_struct ()
    {
        TIME_SECONDS = 0.0;
        TIME_FRACTIONAL = 0.0;
        MANUFACTURER_ID = 0;
        LATITUDE = 0.0;
        LONGITUDE = 0.0;
        ALTITUDE = 0.0;
        GROUND_SPEED = 0.0;
        HEADING_ANGLE = 0.0;
        TRACK_ANGLE = 0.0;
        MAGNETIC_VARIATION = 0.0;
    };

    std::string getId() {
        return std::string("GEOLOCATION_INS");
    };

    double TIME_SECONDS;
    double TIME_FRACTIONAL;
    CORBA::Long MANUFACTURER_ID;
    double LATITUDE;
    double LONGITUDE;
    double ALTITUDE;
    double GROUND_SPEED;
    double HEADING_ANGLE;
    double TRACK_ANGLE;
    double MAGNETIC_VARIATION;
};

inline bool operator>>= (const CORBA::Any& a, GEOLOCATION_INS_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("GEOLOCATION_INS::TIME_SECONDS", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_SECONDS)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::TIME_FRACTIONAL", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_FRACTIONAL)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::MANUFACTURER_ID", props[idx].id)) {
            if (!(props[idx].value >>= s.MANUFACTURER_ID)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::LATITUDE", props[idx].id)) {
            if (!(props[idx].value >>= s.LATITUDE)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::LONGITUDE", props[idx].id)) {
            if (!(props[idx].value >>= s.LONGITUDE)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::ALTITUDE", props[idx].id)) {
            if (!(props[idx].value >>= s.ALTITUDE)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::GROUND_SPEED", props[idx].id)) {
            if (!(props[idx].value >>= s.GROUND_SPEED)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::HEADING_ANGLE", props[idx].id)) {
            if (!(props[idx].value >>= s.HEADING_ANGLE)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::TRACK_ANGLE", props[idx].id)) {
            if (!(props[idx].value >>= s.TRACK_ANGLE)) return false;
        }
        if (!strcmp("GEOLOCATION_INS::MAGNETIC_VARIATION", props[idx].id)) {
            if (!(props[idx].value >>= s.MAGNETIC_VARIATION)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const GEOLOCATION_INS_struct& s) {
    CF::Properties props;
    props.length(10);
    props[0].id = CORBA::string_dup("GEOLOCATION_INS::TIME_SECONDS");
    props[0].value <<= s.TIME_SECONDS;
    props[1].id = CORBA::string_dup("GEOLOCATION_INS::TIME_FRACTIONAL");
    props[1].value <<= s.TIME_FRACTIONAL;
    props[2].id = CORBA::string_dup("GEOLOCATION_INS::MANUFACTURER_ID");
    props[2].value <<= s.MANUFACTURER_ID;
    props[3].id = CORBA::string_dup("GEOLOCATION_INS::LATITUDE");
    props[3].value <<= s.LATITUDE;
    props[4].id = CORBA::string_dup("GEOLOCATION_INS::LONGITUDE");
    props[4].value <<= s.LONGITUDE;
    props[5].id = CORBA::string_dup("GEOLOCATION_INS::ALTITUDE");
    props[5].value <<= s.ALTITUDE;
    props[6].id = CORBA::string_dup("GEOLOCATION_INS::GROUND_SPEED");
    props[6].value <<= s.GROUND_SPEED;
    props[7].id = CORBA::string_dup("GEOLOCATION_INS::HEADING_ANGLE");
    props[7].value <<= s.HEADING_ANGLE;
    props[8].id = CORBA::string_dup("GEOLOCATION_INS::TRACK_ANGLE");
    props[8].value <<= s.TRACK_ANGLE;
    props[9].id = CORBA::string_dup("GEOLOCATION_INS::MAGNETIC_VARIATION");
    props[9].value <<= s.MAGNETIC_VARIATION;
    a <<= props;
};

inline bool operator== (const GEOLOCATION_INS_struct& s1, const GEOLOCATION_INS_struct& s2) {
    if (s1.TIME_SECONDS!=s2.TIME_SECONDS)
        return false;
    if (s1.TIME_FRACTIONAL!=s2.TIME_FRACTIONAL)
        return false;
    if (s1.MANUFACTURER_ID!=s2.MANUFACTURER_ID)
        return false;
    if (s1.LATITUDE!=s2.LATITUDE)
        return false;
    if (s1.LONGITUDE!=s2.LONGITUDE)
        return false;
    if (s1.ALTITUDE!=s2.ALTITUDE)
        return false;
    if (s1.GROUND_SPEED!=s2.GROUND_SPEED)
        return false;
    if (s1.HEADING_ANGLE!=s2.HEADING_ANGLE)
        return false;
    if (s1.TRACK_ANGLE!=s2.TRACK_ANGLE)
        return false;
    if (s1.MAGNETIC_VARIATION!=s2.MAGNETIC_VARIATION)
        return false;
    return true;
};

inline bool operator!= (const GEOLOCATION_INS_struct& s1, const GEOLOCATION_INS_struct& s2) {
    return !(s1==s2);
};

template<> inline short StructProperty<GEOLOCATION_INS_struct>::compare (const CORBA::Any& a) {
    if (super::isNil_) {
        if (a.type()->kind() == (CORBA::tk_null)) {
            return 0;
        }
        return 1;
    }

    GEOLOCATION_INS_struct tmp;
    if (fromAny(a, tmp)) {
        if (tmp != this->value_) {
            return 1;
        }

        return 0;
    } else {
        return 1;
    }
}

struct EPHEMERIS_ECEF_struct {
    EPHEMERIS_ECEF_struct ()
    {
        TIME_SECONDS = 0.0;
        TIME_FRACTIONAL_SECONDS = 0.0;
        POSITION_X = 0.0;
        POSITION_Y = 0.0;
        POSITION_Z = 0.0;
        ATTITUDE_ALPHA = 0.0;
        ATTITUDE_BETA = 0.0;
        ATTITUDE_PHI = 0.0;
        VELOCITY_X = 0.0;
        VELOCITY_Y = 0.0;
        VELOCITY_Z = 0.0;
        ROTATIONAL_VELOCITY_ALPHA = 0.0;
        ROTATIONAL_VELOCITY_BETA = 0.0;
        ROTATIONAL_VELOCITY_PHI = 0.0;
        ACCELERATION_X= 0.0;
        ACCELERATION_Y= 0.0;
        ACCELERATION_Z= 0.0;
        ROTATIONAL_ACCELERATION_ALPHA= 0.0;
        ROTATIONAL_ACCELERATION_BETA= 0.0;
        ROTATIONAL_ACCELERATION_PHI= 0.0;
    };

    std::string getId() {
        return std::string("EPHEMERIS_ECEF");
    };

    double TIME_SECONDS;
    double TIME_FRACTIONAL_SECONDS;
    double POSITION_X;
    double POSITION_Y;
    double POSITION_Z;
    double ATTITUDE_ALPHA;
    double ATTITUDE_BETA;
    double ATTITUDE_PHI;
    double VELOCITY_X;
    double VELOCITY_Y;
    double VELOCITY_Z;
    double ROTATIONAL_VELOCITY_ALPHA;
    double ROTATIONAL_VELOCITY_BETA;
    double ROTATIONAL_VELOCITY_PHI;
    double ACCELERATION_X;
    double ACCELERATION_Y;
    double ACCELERATION_Z;
    double ROTATIONAL_ACCELERATION_ALPHA;
    double ROTATIONAL_ACCELERATION_BETA;
    double ROTATIONAL_ACCELERATION_PHI;
};

inline bool operator>>= (const CORBA::Any& a, EPHEMERIS_ECEF_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("EPHEMERIS_ECEF::TIME_SECONDS", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_SECONDS)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::TIME_FRACTIONAL_SECONDS", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_FRACTIONAL_SECONDS)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::POSITION_X", props[idx].id)) {
            if (!(props[idx].value >>= s.POSITION_X)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::POSITION_Y", props[idx].id)) {
            if (!(props[idx].value >>= s.POSITION_Y)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::POSITION_Z", props[idx].id)) {
            if (!(props[idx].value >>= s.POSITION_Z)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ATTITUDE_ALPHA", props[idx].id)) {
            if (!(props[idx].value >>= s.ATTITUDE_ALPHA)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ATTITUDE_BETA", props[idx].id)) {
            if (!(props[idx].value >>= s.ATTITUDE_BETA)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ATTITUDE_PHI", props[idx].id)) {
            if (!(props[idx].value >>= s.ATTITUDE_PHI)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::VELOCITY_X", props[idx].id)) {
            if (!(props[idx].value >>= s.VELOCITY_X)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::VELOCITY_Y", props[idx].id)) {
            if (!(props[idx].value >>= s.VELOCITY_Y)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::VELOCITY_Z", props[idx].id)) {
            if (!(props[idx].value >>= s.VELOCITY_Z)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ROTATIONAL_VELOCITY_ALPHA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_VELOCITY_ALPHA)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ROTATIONAL_VELOCITY_BETA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_VELOCITY_BETA)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ROTATIONAL_VELOCITY_PHI", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_VELOCITY_PHI)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ACCELERATION_X", props[idx].id)) {
            if (!(props[idx].value >>= s.ACCELERATION_X)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ACCELERATION_Y", props[idx].id)) {
            if (!(props[idx].value >>= s.ACCELERATION_Y)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ACCELERATION_Z", props[idx].id)) {
            if (!(props[idx].value >>= s.ACCELERATION_Z)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ROTATIONAL_ACCELERATION_ALPHA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_ACCELERATION_ALPHA)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ROTATIONAL_ACCELERATION_BETA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_ACCELERATION_BETA)) return false;
        }
        if (!strcmp("EPHEMERIS_ECEF::ROTATIONAL_ACCELERATION_PHI", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_ACCELERATION_PHI)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const EPHEMERIS_ECEF_struct& s) {
    CF::Properties props;
    props.length(20);
    props[0].id = CORBA::string_dup("EPHEMERIS_ECEF::TIME_SECONDS");
    props[0].value <<= s.TIME_SECONDS;
    props[1].id = CORBA::string_dup("EPHEMERIS_ECEF::TIME_FRACTIONAL_SECONDS");
    props[1].value <<= s.TIME_FRACTIONAL_SECONDS;
    props[2].id = CORBA::string_dup("EPHEMERIS_ECEF::POSITION_X");
    props[2].value <<= s.POSITION_X;
    props[3].id = CORBA::string_dup("EPHEMERIS_ECEF::POSITION_Y");
    props[3].value <<= s.POSITION_Y;
    props[4].id = CORBA::string_dup("EPHEMERIS_ECEF::POSITION_Z");
    props[4].value <<= s.POSITION_Z;
    props[5].id = CORBA::string_dup("EPHEMERIS_ECEF::ATTITUDE_ALPHA");
    props[5].value <<= s.ATTITUDE_ALPHA;
    props[6].id = CORBA::string_dup("EPHEMERIS_ECEF::ATTITUDE_BETA");
    props[6].value <<= s.ATTITUDE_BETA;
    props[7].id = CORBA::string_dup("EPHEMERIS_ECEF::ATTITUDE_PHI");
    props[7].value <<= s.ATTITUDE_PHI;
    props[8].id = CORBA::string_dup("EPHEMERIS_ECEF::VELOCITY_X");
    props[8].value <<= s.VELOCITY_X;
    props[9].id = CORBA::string_dup("EPHEMERIS_ECEF::VELOCITY_Y");
    props[9].value <<= s.VELOCITY_Y;
    props[10].id = CORBA::string_dup("EPHEMERIS_ECEF::VELOCITY_Z");
    props[10].value <<= s.VELOCITY_Z;
    props[11].id = CORBA::string_dup("EPHEMERIS_ECEF::ROTATIONAL_VELOCITY_ALPHA");
    props[11].value <<= s.ROTATIONAL_VELOCITY_ALPHA;
    props[12].id = CORBA::string_dup("EPHEMERIS_ECEF::ROTATIONAL_VELOCITY_BETA");
    props[12].value <<= s.ROTATIONAL_VELOCITY_BETA;
    props[13].id = CORBA::string_dup("EPHEMERIS_ECEF::ROTATIONAL_VELOCITY_PHI");
    props[13].value <<= s.ROTATIONAL_VELOCITY_PHI;
    props[14].id = CORBA::string_dup("EPHEMERIS_ECEF::ACCELERATION_X");
    props[14].value <<= s.ACCELERATION_X;
    props[15].id = CORBA::string_dup("EPHEMERIS_ECEF::ACCELERATION_Y");
    props[15].value <<= s.ACCELERATION_Y;
    props[16].id = CORBA::string_dup("EPHEMERIS_ECEF::ACCELERATION_Z");
    props[16].value <<= s.ACCELERATION_Z;
    props[17].id = CORBA::string_dup("EPHEMERIS_ECEF::ROTATIONAL_ACCELERATION_ALPHA");
    props[17].value <<= s.ROTATIONAL_ACCELERATION_ALPHA;
    props[18].id = CORBA::string_dup("EPHEMERIS_ECEF::ROTATIONAL_ACCELERATION_BETA");
    props[18].value <<= s.ROTATIONAL_ACCELERATION_BETA;
    props[19].id = CORBA::string_dup("EPHEMERIS_ECEF::ROTATIONAL_ACCELERATION_PHI");
    props[19].value <<= s.ROTATIONAL_ACCELERATION_PHI;
    a <<= props;
};

inline bool operator== (const EPHEMERIS_ECEF_struct& s1, const EPHEMERIS_ECEF_struct& s2) {
    if (s1.TIME_SECONDS!=s2.TIME_SECONDS)
        return false;
    if (s1.TIME_FRACTIONAL_SECONDS!=s2.TIME_FRACTIONAL_SECONDS)
        return false;
    if (s1.POSITION_X!=s2.POSITION_X)
        return false;
    if (s1.POSITION_Y!=s2.POSITION_Y)
        return false;
    if (s1.POSITION_Z!=s2.POSITION_Z)
        return false;
    if (s1.ATTITUDE_ALPHA!=s2.ATTITUDE_ALPHA)
        return false;
    if (s1.ATTITUDE_BETA!=s2.ATTITUDE_BETA)
        return false;
    if (s1.ATTITUDE_PHI!=s2.ATTITUDE_PHI)
        return false;
    if (s1.VELOCITY_X!=s2.VELOCITY_X)
        return false;
    if (s1.VELOCITY_Y!=s2.VELOCITY_Y)
        return false;
    if (s1.VELOCITY_Z!=s2.VELOCITY_Z)
        return false;
    if (s1.ROTATIONAL_VELOCITY_ALPHA!=s2.ROTATIONAL_VELOCITY_ALPHA)
        return false;
    if (s1.ROTATIONAL_VELOCITY_BETA!=s2.ROTATIONAL_VELOCITY_BETA)
        return false;
    if (s1.ROTATIONAL_VELOCITY_PHI!=s2.ROTATIONAL_VELOCITY_PHI)
        return false;
    if (s1.ACCELERATION_X!=s2.ACCELERATION_X)
        return false;
    if (s1.ACCELERATION_Y!=s2.ACCELERATION_Y)
        return false;
    if (s1.ACCELERATION_Z!=s2.ACCELERATION_Z)
        return false;
    if (s1.ROTATIONAL_ACCELERATION_ALPHA!=s2.ROTATIONAL_ACCELERATION_ALPHA)
        return false;
    if (s1.ROTATIONAL_ACCELERATION_BETA!=s2.ROTATIONAL_ACCELERATION_BETA)
        return false;
    if (s1.ROTATIONAL_ACCELERATION_PHI!=s2.ROTATIONAL_ACCELERATION_PHI)
        return false;
    return true;
};

inline bool operator!= (const EPHEMERIS_ECEF_struct& s1, const EPHEMERIS_ECEF_struct& s2) {
    return !(s1==s2);
};

template<> inline short StructProperty<EPHEMERIS_ECEF_struct>::compare (const CORBA::Any& a) {
    if (super::isNil_) {
        if (a.type()->kind() == (CORBA::tk_null)) {
            return 0;
        }
        return 1;
    }

    EPHEMERIS_ECEF_struct tmp;
    if (fromAny(a, tmp)) {
        if (tmp != this->value_) {
            return 1;
        }

        return 0;
    } else {
        return 1;
    }
}

struct EPHEMERIS_RELATIVE_struct {
    EPHEMERIS_RELATIVE_struct ()
    {
        TIME_SECONDS = 0.0;
        TIME_FRACTIONAL_SECONDS = 0.0;
        POSITION_X = 0.0;
        POSITION_Y = 0.0;
        POSITION_Z = 0.0;
        ATTITUDE_ALPHA = 0.0;
        ATTITUDE_BETA = 0.0;
        ATTITUDE_PHI = 0.0;
        VELOCITY_X = 0.0;
        VELOCITY_Y = 0.0;
        VELOCITY_Z = 0.0;
        ROTATIONAL_VELOCITY_ALPHA = 0.0;
        ROTATIONAL_VELOCITY_BETA = 0.0;
        ROTATIONAL_VELOCITY_PHI = 0.0;
        ACCELERATION_X = 0.0;
        ACCELERATION_Y = 0.0;
        ACCELERATION_Z = 0.0;
        ROTATIONAL_ACCELERATION_ALPHA = 0.0;
        ROTATIONAL_ACCELERATION_BETA = 0.0;
        ROTATIONAL_ACCELERATION_PHI = 0.0;
    };

    std::string getId() {
        return std::string("EPHEMERIS_RELATIVE");
    };

    double TIME_SECONDS;
    double TIME_FRACTIONAL_SECONDS;
    double POSITION_X;
    double POSITION_Y;
    double POSITION_Z;
    double ATTITUDE_ALPHA;
    double ATTITUDE_BETA;
    double ATTITUDE_PHI;
    double VELOCITY_X;
    double VELOCITY_Y;
    double VELOCITY_Z;
    double ROTATIONAL_VELOCITY_ALPHA;
    double ROTATIONAL_VELOCITY_BETA;
    double ROTATIONAL_VELOCITY_PHI;
    double ACCELERATION_X;
    double ACCELERATION_Y;
    double ACCELERATION_Z;
    double ROTATIONAL_ACCELERATION_ALPHA;
    double ROTATIONAL_ACCELERATION_BETA;
    double ROTATIONAL_ACCELERATION_PHI;
};

inline bool operator>>= (const CORBA::Any& a, EPHEMERIS_RELATIVE_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("EPHEMERIS_RELATIVE::TIME_SECONDS", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_SECONDS)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::TIME_FRACTIONAL_SECONDS", props[idx].id)) {
            if (!(props[idx].value >>= s.TIME_FRACTIONAL_SECONDS)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::POSITION_X", props[idx].id)) {
            if (!(props[idx].value >>= s.POSITION_X)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::POSITION_Y", props[idx].id)) {
            if (!(props[idx].value >>= s.POSITION_Y)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::POSITION_Z", props[idx].id)) {
            if (!(props[idx].value >>= s.POSITION_Z)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ATTITUDE_ALPHA", props[idx].id)) {
            if (!(props[idx].value >>= s.ATTITUDE_ALPHA)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ATTITUDE_BETA", props[idx].id)) {
            if (!(props[idx].value >>= s.ATTITUDE_BETA)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ATTITUDE_PHI", props[idx].id)) {
            if (!(props[idx].value >>= s.ATTITUDE_PHI)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::VELOCITY_X", props[idx].id)) {
            if (!(props[idx].value >>= s.VELOCITY_X)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::VELOCITY_Y", props[idx].id)) {
            if (!(props[idx].value >>= s.VELOCITY_Y)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::VELOCITY_Z", props[idx].id)) {
            if (!(props[idx].value >>= s.VELOCITY_Z)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ROTATIONAL_VELOCITY_ALPHA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_VELOCITY_ALPHA)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ROTATIONAL_VELOCITY_BETA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_VELOCITY_BETA)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ROTATIONAL_VELOCITY_PHI", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_VELOCITY_PHI)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ACCELERATION_X", props[idx].id)) {
            if (!(props[idx].value >>= s.ACCELERATION_X)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ACCELERATION_Y", props[idx].id)) {
            if (!(props[idx].value >>= s.ACCELERATION_Y)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ACCELERATION_Z", props[idx].id)) {
            if (!(props[idx].value >>= s.ACCELERATION_Z)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ROTATIONAL_ACCELERATION_ALPHA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_ACCELERATION_ALPHA)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ROTATIONAL_ACCELERATION_BETA", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_ACCELERATION_BETA)) return false;
        }
        if (!strcmp("EPHEMERIS_RELATIVE::ROTATIONAL_ACCELERATION_PHI", props[idx].id)) {
            if (!(props[idx].value >>= s.ROTATIONAL_ACCELERATION_PHI)) return false;
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const EPHEMERIS_RELATIVE_struct& s) {
    CF::Properties props;
    props.length(20);
    props[0].id = CORBA::string_dup("EPHEMERIS_RELATIVE::TIME_SECONDS");
    props[0].value <<= s.TIME_SECONDS;
    props[1].id = CORBA::string_dup("EPHEMERIS_RELATIVE::TIME_FRACTIONAL_SECONDS");
    props[1].value <<= s.TIME_FRACTIONAL_SECONDS;
    props[2].id = CORBA::string_dup("EPHEMERIS_RELATIVE::POSITION_X");
    props[2].value <<= s.POSITION_X;
    props[3].id = CORBA::string_dup("EPHEMERIS_RELATIVE::POSITION_Y");
    props[3].value <<= s.POSITION_Y;
    props[4].id = CORBA::string_dup("EPHEMERIS_RELATIVE::POSITION_Z");
    props[4].value <<= s.POSITION_Z;
    props[5].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ATTITUDE_ALPHA");
    props[5].value <<= s.ATTITUDE_ALPHA;
    props[6].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ATTITUDE_BETA");
    props[6].value <<= s.ATTITUDE_BETA;
    props[7].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ATTITUDE_PHI");
    props[7].value <<= s.ATTITUDE_PHI;
    props[8].id = CORBA::string_dup("EPHEMERIS_RELATIVE::VELOCITY_X");
    props[8].value <<= s.VELOCITY_X;
    props[9].id = CORBA::string_dup("EPHEMERIS_RELATIVE::VELOCITY_Y");
    props[9].value <<= s.VELOCITY_Y;
    props[10].id = CORBA::string_dup("EPHEMERIS_RELATIVE::VELOCITY_Z");
    props[10].value <<= s.VELOCITY_Z;
    props[11].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ROTATIONAL_VELOCITY_ALPHA");
    props[11].value <<= s.ROTATIONAL_VELOCITY_ALPHA;
    props[12].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ROTATIONAL_VELOCITY_BETA");
    props[12].value <<= s.ROTATIONAL_VELOCITY_BETA;
    props[13].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ROTATIONAL_VELOCITY_PHI");
    props[13].value <<= s.ROTATIONAL_VELOCITY_PHI;
    props[14].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ACCELERATION_X");
    props[14].value <<= s.ACCELERATION_X;
    props[15].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ACCELERATION_Y");
    props[15].value <<= s.ACCELERATION_Y;
    props[16].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ACCELERATION_Z");
    props[16].value <<= s.ACCELERATION_Z;
    props[17].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ROTATIONAL_ACCELERATION_ALPHA");
    props[17].value <<= s.ROTATIONAL_ACCELERATION_ALPHA;
    props[18].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ROTATIONAL_ACCELERATION_BETA");
    props[18].value <<= s.ROTATIONAL_ACCELERATION_BETA;
    props[19].id = CORBA::string_dup("EPHEMERIS_RELATIVE::ROTATIONAL_ACCELERATION_PHI");
    props[19].value <<= s.ROTATIONAL_ACCELERATION_PHI;
    a <<= props;
};

inline bool operator== (const EPHEMERIS_RELATIVE_struct& s1, const EPHEMERIS_RELATIVE_struct& s2) {
    if (s1.TIME_SECONDS!=s2.TIME_SECONDS)
        return false;
    if (s1.TIME_FRACTIONAL_SECONDS!=s2.TIME_FRACTIONAL_SECONDS)
        return false;
    if (s1.POSITION_X!=s2.POSITION_X)
        return false;
    if (s1.POSITION_Y!=s2.POSITION_Y)
        return false;
    if (s1.POSITION_Z!=s2.POSITION_Z)
        return false;
    if (s1.ATTITUDE_ALPHA!=s2.ATTITUDE_ALPHA)
        return false;
    if (s1.ATTITUDE_BETA!=s2.ATTITUDE_BETA)
        return false;
    if (s1.ATTITUDE_PHI!=s2.ATTITUDE_PHI)
        return false;
    if (s1.VELOCITY_X!=s2.VELOCITY_X)
        return false;
    if (s1.VELOCITY_Y!=s2.VELOCITY_Y)
        return false;
    if (s1.VELOCITY_Z!=s2.VELOCITY_Z)
        return false;
    if (s1.ROTATIONAL_VELOCITY_ALPHA!=s2.ROTATIONAL_VELOCITY_ALPHA)
        return false;
    if (s1.ROTATIONAL_VELOCITY_BETA!=s2.ROTATIONAL_VELOCITY_BETA)
        return false;
    if (s1.ROTATIONAL_VELOCITY_PHI!=s2.ROTATIONAL_VELOCITY_PHI)
        return false;
    if (s1.ACCELERATION_X!=s2.ACCELERATION_X)
        return false;
    if (s1.ACCELERATION_Y!=s2.ACCELERATION_Y)
        return false;
    if (s1.ACCELERATION_Z!=s2.ACCELERATION_Z)
        return false;
    if (s1.ROTATIONAL_ACCELERATION_ALPHA!=s2.ROTATIONAL_ACCELERATION_ALPHA)
        return false;
    if (s1.ROTATIONAL_ACCELERATION_BETA!=s2.ROTATIONAL_ACCELERATION_BETA)
        return false;
    if (s1.ROTATIONAL_ACCELERATION_PHI!=s2.ROTATIONAL_ACCELERATION_PHI)
        return false;
    return true;
};

inline bool operator!= (const EPHEMERIS_RELATIVE_struct& s1, const EPHEMERIS_RELATIVE_struct& s2) {
    return !(s1==s2);
};

template<> inline short StructProperty<EPHEMERIS_RELATIVE_struct>::compare (const CORBA::Any& a) {
    if (super::isNil_) {
        if (a.type()->kind() == (CORBA::tk_null)) {
            return 0;
        }
        return 1;
    }

    EPHEMERIS_RELATIVE_struct tmp;
    if (fromAny(a, tmp)) {
        if (tmp != this->value_) {
            return 1;
        }

        return 0;
    } else {
        return 1;
    }
}


#endif

/* 
 * Copyright (C) 2013 CoDyCo
 * Author: Andrea Del Prete
 * email:  andrea.delprete@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#ifndef _MOTOR_FRICTION_EXCITATION_PARAMS
#define _MOTOR_FRICTION_EXCITATION_PARAMS

#include <motorFrictionExcitation/motorFrictionExcitationConstants.h>
#include <paramHelp/paramProxyBasic.h>
#include <Eigen/Core>                               // import most common Eigen types
#include <vector>
#include <string>
#include <yarp/os/Bottle.h>

using namespace paramHelp;
using namespace Eigen;
using namespace std;

namespace motorFrictionExcitation
{

///< IDs of all the module parameters
enum MotorFrictionExcitationParamId 
{ 
    PARAM_ID_MODULE_NAME,       PARAM_ID_CTRL_PERIOD,       PARAM_ID_ROBOT_NAME, 
    PARAM_FREE_MOTION_EXCIT,    PARAM_ID_Q_MAX,             PARAM_ID_Q_MIN,             
    PARAM_ID_Q,                 PARAM_ID_PWM_DES,           PARAM_ID_SEND_COMMANDS,
    PARAM_ID_SIZE /*This is the number of parameters, so it must be the last value of the enum.*/
};

///< Ids of the subparameters conposing the struct parameter FreeMotionExcitation
enum FreeMotionExcitationParamId
{
    PARAM_ID_JOINT_ID,          PARAM_ID_INIT_Q,            PARAM_ID_A,
    PARAM_ID_A0,                PARAM_ID_W,                 PARAM_ID_JOINT_LIM_THR,
    PARAM_ID_FRIC_PAR_COV_THR,  PARAM_ID_POS_INT_GAIN,
    FREE_MOTION_EXCITATION_PARAM_ID_SIZE /*This is the number of parameters, so it must be the last value of the enum.*/
};

// ******************************************************************************************************************************
// ****************************************** DESCRIPTION OF ALL THE MODULE PARAMETERS ******************************************
// ******************************************************************************************************************************

class FreeMotionExcitation
{
    std::map<int, ParamProxyInterface*>     paramList;      ///< list of parameter proxies

    /** Add the specified parameters to the list of managed parameters. 
     * If a default value is specified, the parameter is initialized to that value.
     * @param pdList Array of const pointers to const ParamProxyInterface containing a description of the parameters to add.
     * @return True if the operation succeeded, false otherwise (parameter id conflict). 
     * @note This method clones the paramProxyInterface contained in pdList. */
    bool addParams(const ParamProxyInterface *const *pdList, int size);
    
    /** Resize the variable associated to the parameter with the specified id. */
    void resizeParam(int paramId, int newSize);

    void relinkParam(int paramId, int newSize=-1);

public:
    ArrayXi jointId;
    ArrayXd initialJointConfiguration;
    ArrayXd a;
    ArrayXd a0;
    ArrayXd w;
    ArrayXd jointLimitThresh;
    ArrayXd fricParamCovarThresh;
    ArrayXd ki;

    FreeMotionExcitation();
    //FreeMotionExcitation(const FreeMotionExcitation &other);
    FreeMotionExcitation& operator=(const FreeMotionExcitation& rhs);

    bool setFromValue(const yarp::os::Value &v);
    yarp::os::Value getAsValue();

    bool setSubParam(const char *key, const yarp::os::Bottle &value, yarp::os::Bottle *reply=0);
    std::string toString();
};

const ParamProxyInterface *const motorFrictionExcitationParamDescr[PARAM_ID_SIZE]  = 
{
// ************************************************* STRUCT PARAMETERS ****************************************************************************************************************************************************************************************************************************************
//                                        NAME                       ID                          SIZE                I/O ACCESS         DEFAULT VALUE               DESCRIPTION
new ParamProxyClass<FreeMotionExcitation>("free motion excitation",  PARAM_FREE_MOTION_EXCIT,    PARAM_SIZE_FREE,    PARAM_CONFIG,      0,                          "Information describing the excitation process for the joints to move in free motion"),
// ************************************************* SIMPLE PARAMETERS ****************************************************************************************************************************************************************************************************************************************
//                          NAME                    ID                          SIZE                BOUNDS                                      I/O ACCESS          DEFAULT VALUE                   DESCRIPTION
new ParamProxyBasic<string>("name",                 PARAM_ID_MODULE_NAME,       1,                                                              PARAM_CONFIG,       &DEFAULT_MODULE_NAME,           "Name of the instance of the module"), 
new ParamProxyBasic<int>(   "period",               PARAM_ID_CTRL_PERIOD,       1,                  ParamBilatBounds<int>(1,1000),              PARAM_CONFIG,       &DEFAULT_CTRL_PERIOD,           "Period of the control loop (ms)"), 
new ParamProxyBasic<string>("robot",                PARAM_ID_ROBOT_NAME,        1,                                                              PARAM_CONFIG,       &DEFAULT_ROBOT_NAME,            "Name of the robot"), 
// ************************************************* RPC PARAMETERS ****************************************************************************************************************************************************************************************************************************************
new ParamProxyBasic<double>("q max",                PARAM_ID_Q_MAX,             ICUB_DOFS,          ParamBilatBounds<double>(-360.0,360.0),     PARAM_IN_OUT,       DEFAULT_Q_MAX.data(),           "Joint upper bounds"),
new ParamProxyBasic<double>("q min",                PARAM_ID_Q_MIN,             ICUB_DOFS,          ParamBilatBounds<double>(-360.0,360.0),     PARAM_IN_OUT,       DEFAULT_Q_MIN.data(),           "Joint lower bounds"),
new ParamProxyBasic<int>(   "send commands",        PARAM_ID_SEND_COMMANDS,     1,                  ParamBilatBounds<int>(0, 1),                PARAM_IN_OUT,       &DEFAULT_SEND_COMMANDS,         "Specify whether to send commands to the motors"),
// ************************************************* STREAMING OUTPUT PARAMETERS ****************************************************************************************************************************************************************************************************************************
// ************************************************* STREAMING MONITOR PARAMETERS ****************************************************************************************************************************************************************************************************************************
new ParamProxyBasic<double>("q",                    PARAM_ID_Q,                 1,                  ParamBilatBounds<double>(-150.0, 150.0),    PARAM_MONITOR,      0,                              "Joint angle of the currently controlled motor"),
new ParamProxyBasic<double>("pwmDes",               PARAM_ID_PWM_DES,           1,                  ParamBilatBounds<double>(-1333.0, 1333.0),  PARAM_MONITOR,      0,                              "Current desired pwm sent to the motors")
};

}   // end namespace 

#endif

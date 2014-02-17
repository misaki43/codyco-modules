/* 
 * Copyright (C) 2013 CoDyCo
 * Author: Daniele Pucci
 * email:  daniele.pucci@iit.it
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

/**
 * @ingroup codyco_module
 * \defgroup codyco_jointTorqueControl jointTorqueControl
 *
 * This module implements the whole-body torque control.
 *
 * \section intro_sec Background
 *
 * The robot's motors are controlled by using 
 * the PWM technique, i.e. the tension applied to each motor is a square wave of constant frequency and amplitude, 
 * and the duty cycle of this wave is used as control input. 

    V(t) ^           Tdc
             |         /-----/ 
          Vb | _  _  _  _____             ____ _
             |         |     |           |
             |         |     |           |
             |         |     |           |
             |_________|_____|___________|_______ 
                       /-----------------/       t
                                  T 

where
* \f$T\f$: period of the square wave;
* \f$V\f$: tension applied to the motor;
* \f$V_b\f$: (positive) amplitude of the square wave;
* \f$T_{dc}\f$: duty cycle time.

The relationship between the link's torque \f$ \tau \f$ and the tension \f$ V \f$ applied to the motor is assumed to be:
\f[
	V  = k_t \tau + k_v \dot{q} + k_c \text{sign}(\dot{q}),
\f]
with \f$k_t\f$, \f$k_v\f$, \f$k_c\f$ three constants, and \f$\dot{q}\f$ the link's velocity. 
Since the tension \f$V(t)\f$ is a high-frequency square 
wave, we can assume that the above relationship holds for \f$V = V_m\f$, where \f$V_m\f$ stands for the "mean tension value" 
over the time period \f$T\f$. By direct calculations, one can verify that 
\f[
	V_m = (T_{dc}/T) V_b. 
\f]
Also, discontinuities are always challenging in practice. So, it is best to smooth the sign function. 
Among an infinite possible choices, we choose the hyperbolic function instead of \f$\mbox{sign(.)}\f$. Then one has:
\f[
	    V_m  = k_t \tau + k_v \dot{q} + k_c \tanh(k_s \dot{q}).
\f]
This model can be improved by considering possible parameters' asymmetries with respect to the joint velocity \f$\dot{q}\f$.
In particular, the parameters \f$k_v\f$ and \f$k_c\f$ may depend on the sign of \f$\dot{q}\f$, and have different 
values depending on this sign. Then, an improved model is:
\f[
	V_m  = k_t \tau + [k_{vp} s(\dot{q}) + k_{vn} s(-\dot{q})] \dot{q} + [k_{cp} s(\dot{q}) + k_{cn} s(-\dot{q})] \tanh(k_s \dot{q}),
\f]
where the function \f$s(x)\f$ is the step function, i.e.
\f[
	s(x) =  1 \quad \mbox{if} \quad x >= 0; s(x)=0 \quad \mbox{if} \quad x < 0.
\f]
As stated, the above equation constitutes the relation between the tension applied to the motor and the link torque. 
Then, to generate a desired torque \f$\tau_d\f$ coming from an higher control loop, it suffices to evaluate the above equation
with \f$\tau = \tau_d\f$. In practice, however, it is a best practice to add a lower loop to generate \f$\tau\f$ so that \f$\tau\f$ 
will converge to \f$\tau_d\f$, i.e:
\f[
    \tau = \tau_d - k_p e_{\tau} - k_i \int e_{\tau} \mbox{dt},
\f]
where \f$ e_{\tau} := \tau - \tau_d \f$.

\section intro_sec To do and warning list

a) Syncronization between aJ and taoD;
b) Anti wind-up and associated parameters;
c) Observer and a.p.;
d) Filtering parameters for velocity estimation and torque measurement;
	    
\section lib_sec Libraries
YARP.

\section parameters_sec Parameters

\section conf_file_sec Configuration Files
None.

\section tested_os_sec Tested OS Linux.

\section example_sec Example Instantiation of the Module

\author Daniele Pucci 

Copyright (C) 2013 CoDyCo Project

CopyPolicy: Released under the terms of the GNU GPL v2.0.

This file can be edited at CODYCO_HOME/src/modules/jointTorqueControl/include/jointTorqueControl/jointTorqueControlModule.h.
**/


#ifndef __JOINT_TORQUE_CONTROL_MODULE_H__
#define __JOINT_TORQUE_CONTROL_MODULE_H__

#include <iostream>
#include <string>

#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Vocab.h>

#include <paramHelp/paramHelperClient.h>
#include <paramHelp/paramHelperServer.h>
#include <wbiIcub/wholeBodyInterfaceIcub.h>
#include <jointTorqueControl/jointTorqueControlThread.h>
 
using namespace std;
using namespace yarp::os; 
using namespace paramHelp;
using namespace wbi;

namespace jointTorqueControl
{

class jointTorqueControlModule: public RFModule, public CommandObserver
{
    /* module parameters */
    string  moduleName;
    string  robotName;
    string  locoCtrlName;
    string  fileName;
    int     period;

    Port                        rpcPort;	// a port to handle rpc messages
    jointTorqueControlThread*   ctrlThread;     // locomotion control thread
    ParamHelperClient*          torqueCtrl;     // helper class for communicating with the locomotion controller
    ParamHelperServer*          paramHelper;    // helper class for communication
    wholeBodyInterface*         robotInterface; // interface to communicate with the robot

public:
    jointTorqueControlModule();

    bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
    bool interruptModule();                       // interrupt, e.g., the ports 
    bool close();                                 // close and shut down the module
    bool respond(const Bottle& command, Bottle& reply);
    double getPeriod(){ return 0.1; }
    bool updateModule();

    void commandReceived(const CommandDescription &cd, const Bottle &params, Bottle &reply);

};

}

#endif
//empty line to make gcc happy


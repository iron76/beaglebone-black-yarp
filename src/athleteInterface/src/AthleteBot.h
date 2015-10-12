// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2007 RobotCub Consortium
 * Authors: Paul Fitzpatrick and Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <yarp/dev/DeviceDriver.h>

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/FrameGrabberInterfaces.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Time.h>

namespace yarp {
    namespace dev {
        class AthleteBot;
    }
}

class yarp::dev::AthleteBot : public DeviceDriver,
public IPositionControl,
public IVelocityControl,
public IAmplifierControl,
public IEncodersTimed,
public IFrameGrabberImage,
public IControlCalibration2,
public IControlLimits,
public IInteractionMode,
public IControlMode,
public DeviceResponder,
public yarp::os::Thread
{
private:
    int m_njoints;
    double m_x, m_y;
    double m_dx, m_dy;
    double m_tx, m_ty;
    double m_tdx, m_tdy;
    int m_w, m_h;
    double noiseLevel;
    yarp::sig::Vector m_referencePositions, m_referenceVelocities, m_trajectoryGenerationReferenceSpeed, m_trajectoryGenerationReferenceAcc, m_positions;
    yarp::sig::ImageOf<yarp::sig::PixelRgb> back, fore;
    
    void init();
public:
    AthleteBot() {
        m_njoints = 2;
        m_w = 128;
        m_h = 128;
        m_referencePositions.size(m_njoints);                 /* desired reference positions */
        m_referenceVelocities.size(m_njoints);                /* desired reference speeds */
        m_trajectoryGenerationReferenceSpeed.size(m_njoints); /* desired speed for trj generation */
        m_trajectoryGenerationReferenceAcc.size(m_njoints);   /* desired acc for vel generation */
        m_positions.size(m_njoints);
        for (int i=0; i<m_njoints; i++) {
            m_referencePositions[i] = 0;
            m_referenceVelocities[i] = 0;
            m_trajectoryGenerationReferenceSpeed[i] = 0;
            m_trajectoryGenerationReferenceAcc[i] = 0;
            m_positions[i] = 0;
        }
        init();
    }
    
    virtual bool open(yarp::os::Searchable& config);
    
    // IFrameGrabberImage
    virtual bool getImage(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image);
    
    virtual int height() const {
        return m_h;
    }
    
    virtual int width() const {
        return m_w;
    }
    
    
    
    // IPositionControl etc.
    
    virtual bool getAxes(int *ax) {
        *ax = m_njoints;
        return true;
    }
    
    virtual bool setPositionMode() {
        return true;
    }
    
    virtual bool positionMove(int j, double ref) {
        if (j<m_njoints) {
            m_referencePositions[j] = ref;
        }
        return true;
    }
    
    
    virtual bool positionMove(const double *refs) {
        for (int i=0; i<m_njoints; i++) {
            m_referencePositions[i] = refs[i];
        }
        return true;
    }
    
    
    virtual bool relativeMove(int j, double delta) {
        return true;
    }
    
    
    virtual bool relativeMove(const double *deltas) {
        return true;
    }
    
    
    virtual bool checkMotionDone(int j, bool *flag) {
        return true;
    }
    
    
    virtual bool checkMotionDone(bool *flag) {
        return true;
    }
    
    
    virtual bool setRefSpeed(int j, double sp) {
        if (j<m_njoints) {
            m_trajectoryGenerationReferenceSpeed[j] = sp;
        }
        return true;
    }
    
    
    virtual bool setRefSpeeds(const double *spds) {
        for (int i=0; i<m_njoints; i++) {
            m_trajectoryGenerationReferenceSpeed[i] = spds[i];
        }
        return true;
    }
    
    
    virtual bool setRefAcceleration(int j, double acc) {
        if (j<m_njoints) {
            this->m_trajectoryGenerationReferenceAcc[j] = acc;
        }
        return true;
    }
    
    
    virtual bool setRefAccelerations(const double *accs) {
        for (int i=0; i<m_njoints; i++) {
            m_trajectoryGenerationReferenceAcc[i] = accs[i];
        }
        return true;
    }
    
    
    virtual bool getRefSpeed(int j, double *ref) {
        if (j<m_njoints) {
            (*ref) = m_trajectoryGenerationReferenceSpeed[j];
        }
        return true;
    }
    
    
    virtual bool getRefSpeeds(double *spds) {
        for (int i=0; i<m_njoints; i++) {
            spds[i] = m_trajectoryGenerationReferenceSpeed[i];
        }
        return true;
    }
    
    
    virtual bool getRefAcceleration(int j, double *acc) {
        if (j<m_njoints) {
            (*acc) = this->m_trajectoryGenerationReferenceAcc[j];
        }
        return true;
    }
    
    
    virtual bool getRefAccelerations(double *accs) {
        for (int i=0; i<m_njoints; i++) {
            accs[i] = m_trajectoryGenerationReferenceAcc[i];
        }
        return true;
    }
    
    
    virtual bool stop(int j) {
        return true;
    }
    
    
    virtual bool stop() {
        return true;
    }
    
    
    virtual bool close() {
        return true;
    }
    
    virtual bool resetEncoder(int j) {
        return true;
    }
    
    virtual bool resetEncoders() {
        for (int i=0; i<m_njoints; i++) {
            m_referencePositions[i] = 0;
        }
        return true;
    }
    
    virtual bool setEncoder(int j, double val) {
        if (j<m_njoints) {
            m_referencePositions[j] = val;
        }
        return true;
    }
    
    virtual bool setEncoders(const double *vals) {
        for (int i=0; i<m_njoints; i++) {
            m_referencePositions[i] = vals[i];
        }
        return true;
    }
    
    virtual bool getEncoder(int j, double *v) {
        if (j<m_njoints) {
            (*v) = m_positions[j];
        }
        
        return true;
    }
    
    virtual bool getEncoders(double *encs) {
        for (int i=0; i<m_njoints; i++) {
            encs[i] = m_positions[i];
        }
        return true;
    }
    
    virtual bool getEncoderSpeed(int j, double *sp) {
        if (j<m_njoints) {
            (*sp) = 0;
        }
        return true;
    }
    
    virtual bool getEncoderSpeeds(double *spds) {
        for (int i=0; i<m_njoints; i++) {
            spds[i] = 0;
        }
        return true;
    }
    
    virtual bool getEncoderAcceleration(int j, double *spds) {
        if (j<m_njoints) {
            (*spds) = 0;
        }
        return true;
    }
    
    virtual bool getEncoderAccelerations(double *accs) {
        for (int i=0; i<m_njoints; i++) {
            accs[i] = 0;
        }
        return true;
    }
    
    
    // IEncodersTimed
    virtual bool getEncodersTimed(double *encs, double *time)
    {
        bool ret = getEncoders(encs);
        double myTime = yarp::os::Time::now();
        
        for (int i=0; i<m_njoints; i++)
        {
            time[i] = myTime;
        }
        return ret;
    }
    
    virtual bool getEncoderTimed(int j, double *enc, double *time)
    {
        bool ret = getEncoder(j, enc);
        *time = yarp::os::Time::now();
        return ret;
    }
    
    
    virtual bool setVelocityMode() {
        return true;
    }
    
    virtual bool velocityMove(int j, double sp) {
        if (j<m_njoints) {
            m_referenceVelocities[j] = sp;
        }
        return true;
    }
    
    virtual bool velocityMove(const double *sp) {
        for (int i=0; i<m_njoints; i++) {
            m_referenceVelocities[i] = sp[i];
        }
        return true;
    }
    
    
    
    virtual bool enableAmp(int j) {
        return true;
    }
    
    virtual bool disableAmp(int j) {
        return true;
    }
    
    virtual bool getCurrent(int j, double *val) {
        return true;
    }
    
    virtual bool getCurrents(double *vals) {
        return true;
    }
    
    virtual bool getMaxCurrent(int j, double* v) {
        *v = 0;
        return true;
    }
    
    virtual bool setMaxCurrent(int j, double v) {
        return true;
    }
    
    virtual bool getAmpStatus(int *st) {
        *st = 0;
        return true;
    }
    
    virtual bool getAmpStatus(int k, int *v)
    {
        *v=0;
        return true;
    }
    
    virtual bool calibrate2(int j, unsigned int iv, double v1, double v2, double v3)
    {
        fprintf(stderr, "AthleteBot: calibrating joint %d with parameters %u %lf %lf %lf\n", j, iv, v1, v2, v3);
        return true;
    }
    
    virtual bool done(int j)
    {
        fprintf(stderr , "AthleteBot: calibration done on joint %d.\n", j);
        return true;
    }
    
    virtual bool getLimits(int axis, double *min, double *max)
    {
        fprintf(stderr, "AthleteBot: get limits\n");
        *min=0;
        *max=0;
        return true;
    }
    
    virtual bool setLimits(int axis, double min, double max)
    {
        fprintf(stderr, "AthleteBot: set limits\n");
        return true;
    }

    //IInteractionMode
    virtual bool getInteractionMode(int axis, yarp::dev::InteractionModeEnum* mode)
    {
        *mode = (yarp::dev::InteractionModeEnum) VOCAB_IM_STIFF;
        return true;
    }
    
    virtual bool getInteractionModes(int n_joints, int *joints, yarp::dev::InteractionModeEnum* modes)
    {
        if (!modes) return false;
        for(int i=0; i<n_joints; i++)
            modes[i] = (yarp::dev::InteractionModeEnum) VOCAB_IM_STIFF;
        return true;
    }
    
    virtual bool getInteractionModes(yarp::dev::InteractionModeEnum* modes)
    {
        for (int i=0; i<m_njoints; i++) {
            modes[i] = (yarp::dev::InteractionModeEnum) VOCAB_IM_STIFF;
        }
        return true;
    }
    
    virtual bool setInteractionMode(int axis, yarp::dev::InteractionModeEnum mode)
    {
        return true;
    }
    
    virtual bool setInteractionModes(int n_joints, int *joints, yarp::dev::InteractionModeEnum* modes)
    {
        return true;
    }
    
    virtual bool setInteractionModes(yarp::dev::InteractionModeEnum* modes)
    {
        return true;
    }
    //IControlMode
    virtual bool setPositionMode(int j)
    {
        return true;
    }
    
    virtual bool setVelocityMode(int j)
    {
        return true;
    }
    
    virtual bool setTorqueMode(int j)
    {
        return true;
    }
    
    virtual bool setImpedancePositionMode(int j)
    {
        return true;
    }
    
    virtual bool setImpedanceVelocityMode(int j)
    {
        return true;
    }
    
    virtual bool setOpenLoopMode(int j)
    {
        return true;
    }
    
    virtual bool getControlMode(int j, int *mode)
    {
        *mode = (yarp::dev::InteractionModeEnum) VOCAB_CM_POSITION;
        return true;
    }
    
    virtual bool getControlModes(int *modes)
    {
        for (int i=0; i<m_njoints; i++) {
            modes[i] = (yarp::dev::InteractionModeEnum) VOCAB_CM_POSITION;
        }
        return true;
    }
    
    virtual void run();
};


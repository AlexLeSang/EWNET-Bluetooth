#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Defines.hpp"
#include "Message.hpp"
#include "Rand.hpp"

/*!
 * \brief The Environment class
 */
class Environment {
public:
    static Environment & getInstance();
    ~Environment();

    void push( const Message & message );
    void transmit();
    Message pop();

    static void setErrorProbability( qreal probability );
    static void setErrorProbabilityFrom( qreal probability );
    static void setErrorProbabilityTo( qreal probability );
    static void setErrorProbabilityStep( qreal probability );

    static qreal getErrorProbability();
    static qreal getErrorProbabilityFrom();
    static qreal getErrorProbabilityTo();
    static qreal getErrorProbabilityStep();

    void reinit();

    // Forbidden
    Environment( const Environment &  ) = delete;
    Environment & operator = ( const Environment & ) = delete;
    bool operator == ( const Environment &  ) = delete;
    bool operator != ( const Environment &  ) = delete;

private:
    explicit Environment();
    static quint32 corrupt( Message & message );

private:
    static qreal channelErrorProbability;

    static qreal channelErrorProbabilityFrom;
    static qreal channelErrorProbabilityTo;
    static qreal channelErrorProbabilityStep;

    Message transmitedMessage;
};

#endif // ENVIRONMENT_HPP

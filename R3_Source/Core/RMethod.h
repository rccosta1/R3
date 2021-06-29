#ifndef RMethod_H
#define RMethod_H

#include "RObject.h"
#include "RDomain.h"

#include <memory>
#include <QVector>

//using std::make_unique;
//using std::unique_ptr;

class RMethod : public RObject
{
    Q_OBJECT

    Q_PROPERTY(ROutputLevelType OutputLevel READ getOutputLevel WRITE setOutputLevel)

public:

	RMethod(QObject *parent, QString name);
	virtual ~RMethod();

protected:

private:
	
};

#endif

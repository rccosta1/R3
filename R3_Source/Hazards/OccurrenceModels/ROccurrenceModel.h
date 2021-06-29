#ifndef ROccurrenceModel_H
#define ROccurrenceModel_H	
	
#include "RModel.h"	
	
class RParameter;	
class RResponse;
class RLocation;
	
class ROccurrenceModel : public RModel
{	
	Q_OBJECT

public:	
	ROccurrenceModel(QObject *parent, QString name);
    ~ROccurrenceModel();

protected:

};
	
#endif

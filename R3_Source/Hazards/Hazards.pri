
INCLUDEPATH += ./Extended/Hazards \
    ./Extended/Hazards/GroundMotionModels \
    ./Extended/Hazards/IntensityModels \
    ./Extended/Hazards/MagnitudeModels \
    ./Extended/Hazards/LocationModels \
    ./Extended/Hazards/Tools \
    ./Extended/Hazards/OccurrenceModels \
    ./Extended/Hazards/WindAndSnow \
    ./Extended/Hazards/RDomainAdd

HEADERS +=  ./Extended/Hazards/GroundMotionModels/RGroundMotionModel.h \
    ./Extended/Hazards/GroundMotionModels/RRampLoadModel.h \
    ./Extended/Hazards/GroundMotionModels/RScaledGroundMotionModel.h \
    ./Extended/Hazards/GroundMotionModels/RSineLoadModel.h \
    ./Extended/Hazards/GroundMotionModels/RSyntheticGroundMotionModel.h \
    ./Extended/Hazards/IntensityModels/RIntensityModel.h \
    ./Extended/Hazards/IntensityModels/RAtkinson1997IntensityModel.h \
    ./Extended/Hazards/IntensityModels/RAtkinsonBoore2003IntensityModel.h \
    ./Extended/Hazards/IntensityModels/RBooreAtkinson2008IntensityModel.h \
    ./Extended/Hazards/IntensityModels/RCornell1979IntensityModel.h \
    ./Extended/Hazards/IntensityModels/RGivenIntensityModel.h \
    ./Extended/Hazards/IntensityModels/RPGDIntensityModel.h \
    ./Extended/Hazards/IntensityModels/RGenericAttenuationModel.h \
    ./Extended/Hazards/IntensityModels/RJoynerBoore1981IntensityModel.h \
    ./Extended/Hazards/MagnitudeModels/RBoundedExponentialMagnitudeModel.h \
    ./Extended/Hazards/MagnitudeModels/RMomentMagnitudeModel.h \
    ./Extended/Hazards/MagnitudeModels/RUserDefinedMagnitudeModel.h \
    ./Extended/Hazards/MagnitudeModels/RGivenMagnitudeModel.h \
    ./Extended/Hazards/LocationModels/RCircularAreaSourceModel.h \
    ./Extended/Hazards/LocationModels/RMultipointLineSourceModel.h \
    ./Extended/Hazards/LocationModels/RPolygonalAreaSourceModel.h \
    ./Extended/Hazards/LocationModels/RQuadrilateralAreaSourceModel.h \
    ./Extended/Hazards/LocationModels/RRectangularAreaSourceModel.h \
    ./Extended/Hazards/LocationModels/RSingleLineSourceModel.h \
    ./Extended/Hazards/LocationModels/RFixedSourceModel.h \
    ./Extended/Hazards/OccurrenceModels/ROccurrenceModel.h \
    ./Extended/Hazards/OccurrenceModels/RPoissonPointProcessOccurrenceModel.h \
    ./Extended/Hazards/OccurrenceModels/RFixedTimeOccurrenceModel.h \
    ./Extended/Hazards/RLoadBaseClass.h \
    ./Extended/Hazards/REarthquakeBaseClass.h \
    ./Extended/Hazards/RSnowBaseClass.h \
    ./Extended/Hazards/RWindBaseClass.h \
    ./Extended/Hazards/WindAndSnow/RSimpleSnowLoadModel.h \
    ./Extended/Hazards/WindAndSnow/RSimpleWindPressureModel.h \



SOURCES += ./Extended/Hazards/GroundMotionModels/RGroundMotionModel.cpp \
    ./Extended/Hazards/GroundMotionModels/RRampLoadModel.cpp \
    ./Extended/Hazards/GroundMotionModels/RScaledGroundMotionModel.cpp \
    ./Extended/Hazards/GroundMotionModels/RSineLoadModel.cpp \
    ./Extended/Hazards/GroundMotionModels/RSyntheticGroundMotionModel.cpp \
    ./Extended/Hazards/IntensityModels/RIntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RAtkinson1997IntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RAtkinsonBoore2003IntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RBooreAtkinson2008IntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RCornell1979IntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RGivenIntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RPGDIntensityModel.cpp \
    ./Extended/Hazards/IntensityModels/RGenericAttenuationModel.cpp \
    ./Extended/Hazards/IntensityModels/RJoynerBoore1981IntensityModel.cpp \
    ./Extended/Hazards/MagnitudeModels/RBoundedExponentialMagnitudeModel.cpp \
    ./Extended/Hazards/MagnitudeModels/RMomentMagnitudeModel.cpp \
    ./Extended/Hazards/MagnitudeModels/RUserDefinedMagnitudeModel.cpp \
    ./Extended/Hazards/MagnitudeModels/RGivenMagnitudeModel.cpp \
    ./Extended/Hazards/LocationModels/RCircularAreaSourceModel.cpp \
    ./Extended/Hazards/LocationModels/RMultipointLineSourceModel.cpp \
    ./Extended/Hazards/LocationModels/RPolygonalAreaSourceModel.cpp \
    ./Extended/Hazards/LocationModels/RQuadrilateralAreaSourceModel.cpp \
    ./Extended/Hazards/LocationModels/RRectangularAreaSourceModel.cpp \
    ./Extended/Hazards/LocationModels/RSingleLineSourceModel.cpp \
    ./Extended/Hazards/LocationModels/RFixedSourceModel.cpp \
    ./Extended/Hazards/OccurrenceModels/ROccurrenceModel.cpp \
    ./Extended/Hazards/OccurrenceModels/RPoissonPointProcessOccurrenceModel.cpp \
    ./Extended/Hazards/OccurrenceModels/RFixedTimeOccurrenceModel.cpp \
    ./Extended/Hazards/RLoadBaseClass.cpp \
    ./Extended/Hazards/REarthquakeBaseClass.cpp \
    ./Extended/Hazards/RSnowBaseClass.cpp \
    ./Extended/Hazards/RWindBaseClass.cpp \
    ./Extended/Hazards/WindAndSnow/RSimpleSnowLoadModel.cpp \
    ./Extended/Hazards/WindAndSnow/RSimpleWindPressureModel.cpp


HEADERS += ./Extended/Hazards/RDomainAdd/HazardIncludes.h \
           ./Extended/Hazards/RDomainAdd/CreateHazardObjects.h \
           ./Extended/Hazards/RDomainAdd/HazardMenu.h







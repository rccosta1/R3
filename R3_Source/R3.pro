QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl printsupport
greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++14

DEFINES += SIMPLEBUILDINGS
DEFINES += NETWORKS
DEFINES += HAZARDS

#C++14 support
CONFIG += c++14
TEMPLATE = app
#ICON = ./Core/RIcon/RtsIcon.icns

TRANSLATIONS += \
    R3_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    RMainWindow.cpp \
    ./Core/RObjectWidget.cpp \
    ./Core/RDomain.cpp \
    ./Core/RObject.cpp \
    ./Core/RParameter.cpp \
    ./Core/RLocation.cpp \
    ./Core/RLocationResponse.cpp \
    ./Core/RTime.cpp \
    ./Core/RConstant.cpp \
    ./Core/RModel.cpp \
    ./Core/RScenarioModel.cpp \
    ./Core/RModelManager.cpp \
    ./Core/RResponse.cpp \
    ./Core/RGenericResponse.cpp \
    ./Core/RMethod.cpp \
    ./Core/RPropertyWidget.cpp\
    ./Core/RRandomNumberGenerator.cpp\
    ./Core/RInHouseRandomNumberGenerator.cpp\
    ./Core/RNormalDistribution.cpp\
    ./Core/RFragilityFunctions.cpp\
    ./Core/RProbabilityDistributions.cpp\
    ./Core/RInHouseProbabilityDistributions.cpp\
    ./Core/RProbabilityTransformation.cpp\
    ./Core/RIndependenceProbabilityTransformation.cpp\
    ./Core/RRandomVariable.cpp\
    ./Core/RContinuousRandomVariable.cpp\
    ./Core/RLinearSolver.cpp\
    ./Core/RInHouseLinearSolver.cpp\
    ./Core/qcustomplot.cpp \
    ./Core/FlowchartWidget/RFlowchartWidget.cpp \
    ./Core/RDelegate/RComboBoxTableDelegate.cpp \
    ./Core/RDelegate/RCheckBoxTableDelegate.cpp \
    ./Core/RDelegate/RDateTimeDelegate.cpp \
    #./Core/RDelegate/RColorComboBoxTableDelegate.cpp \
    ./Hazards/REarthquakeBaseClass.cpp\
    ./Hazards/IntensityModels/RIntensityModel.cpp\
    ./Hazards/IntensityModels/RPGDIntensityModel.cpp\
    ./Hazards/OccurrenceModels/RFixedTimeOccurrenceModel.cpp\
    ./SimpleBuildings/Models/RHazusBuildingModel.cpp \
    ./Networks/Repositories/RBuildingPortfolio.cpp \
    ./Networks/Repositories/RHAZUSBuildingPortfolio.cpp \
    ./Networks/Repositories/RHouseholdPortfolio.cpp \
    ./Networks/Models/RNetworkModel.cpp \
    ./Networks/Models/Buildings/RNeighborhoodModel.cpp \
    ./Networks/Models/Buildings/RResidentialMobilityNeighborhoodModel.cpp \
    ./Networks/Models/Buildings/RWorkforceStudyNeighborhoodModel.cpp \
    ./Networks/Models/Buildings/RMainNeighborhoodModel.cpp \
    ./Networks/Models/Orchestrating/RNetworkManagerModel.cpp \
    ./Networks/Models/Orchestrating/RNetworkSamplingModel.cpp \
    ./Networks/Parameters/RPacket.cpp \
    ./Networks/Parameters/RPacketResponse.cpp \
    ./Networks/Parameters/RRequest.cpp \
    ./Networks/Parameters/RFinancingRequest.cpp \
    ./Networks/Parameters/RContractorNegotiationRequest.cpp \
    ./Networks/Parameters/REngineeringAssessmentRequest.cpp \
    ./Networks/Parameters/RSingleFamilyContractorRequest.cpp \
    ./Networks/Parameters/RMultiFamilyContractorRequest.cpp \
    ./Networks/Parameters/RPermitRequest.cpp \
    ./Networks/Estimators/RPopulationDisplacementEstimator.cpp \
    ./Networks/Estimators/RAHSPopulationDisplacementEstimator.cpp \
    ./Networks/Estimators/RAHSPreparednessEstimator.cpp \
    ./Networks/Estimators/RSutley2016MorbidityEstimator.cpp \
    ./Networks/Estimators/RAlisjahbana2021FinancingTimeEstimator.cpp \
    ./Networks/Estimators/RDisasterPreparednessEstimator.cpp \
    ./Networks/Models/Businesses/RSingleFamilyContractorFirmModel.cpp \
    ./Networks/Models/Businesses/RMultiFamilyContractorFirmModel.cpp \
    ./Networks/Models/Businesses/RContractorNegotiationModel.cpp \
    ./Networks/Models/Businesses/REngineeringFirmModel.cpp \
    ./Networks/Models/Businesses/RInspectorModel.cpp \
    ./Networks/Models/Businesses/RPermitAssessorModel.cpp \
    ./Networks/Models/Businesses/RPrivateMoneyLenderModel.cpp \
    ./Networks/Models/Businesses/RPublicMoneyLenderModel.cpp \
    ./Networks/Models/Businesses/RGrantProviderModel.cpp \
    ./Networks/Models/Businesses/RInsuranceCompanyModel.cpp \
    ./Networks/Models/Businesses/RGenericMaterialsSupplierModel.cpp \
    ./Networks/Models/Businesses/RHardwareStoreModel.cpp \
    ./Networks/Models/Businesses/RConcretePlantModel.cpp \


HEADERS += \
    RMainWindow.h \
    ./Core/RObjectWidget.h \
    ./Core/RDomain.h \
    ./Core/RObject.h \
    ./Core/RParameter.h \
    ./Core/RLocation.h \
    ./Core/RLocationResponse.h \
    ./Core/RTime.h \
    ./Core/RConstant.h \
    ./Core/RModel.h \
    ./Core/RScenarioModel.h \
    ./Core/RModelManager.h \
    ./Core/RResponse.h \
    ./Core/RGenericResponse.h \
    ./Core/RMethod.h \
    ./Core/RPropertyWidget.h\
    ./Core/RRandomNumberGenerator.h\
    ./Core/RInHouseRandomNumberGenerator.h\
    ./Core/RNormalDistribution.h\
    ./Core/RFragilityFunctions.h\
    ./Core/RProbabilityDistributions.h\
    ./Core/RInHouseProbabilityDistributions.h\
    ./Core/RProbabilityTransformation.h\
    ./Core/RIndependenceProbabilityTransformation.h\
    ./Core/RRandomVariable.h\
    ./Core/RContinuousRandomVariable.h\
    ./Core/RLinearSolver.h\
    ./Core/RInHouseLinearSolver.h\
    ./Core/qcustomplot.h \
    ./Core/FlowchartWidget/RFlowchartWidget.h \
    ./Core/RDelegate/RComboBoxTableDelegate.h \
    ./Core/RDelegate/RCheckBoxTableDelegate.h \
    ./Core/RDelegate/RDateTimeDelegate.h \
    #./Core/RDelegate/RColorComboBoxTableDelegate.h \
    ./Hazards/REarthquakeBaseClass.h\
    ./Hazards/IntensityModels/RIntensityModel.h\
    ./Hazards/IntensityModels/RPGDIntensityModel.h\
    ./Hazards/OccurrenceModels/RFixedTimeOccurrenceModel.h\
    ./SimpleBuildings/RDomainAdd/SimpleBuildingMenu.h \
    ./SimpleBuildings/Models/RHazusBuildingModel.h \
    ./Networks/RDomainAdd/NetworkMenu.h \
    ./Networks/Repositories/RBuildingPortfolio.h \
    ./Networks/Repositories/RHAZUSBuildingPortfolio.h \
    ./Networks/Repositories/RHouseholdPortfolio.h \
    ./Networks/Models/RNetworkModel.h \
    ./Networks/Models/Buildings/RNeighborhoodModel.h \
    ./Networks/Models/Buildings/RResidentialMobilityNeighborhoodModel.h \
    ./Networks/Models/Buildings/RWorkforceStudyNeighborhoodModel.h \
    ./Networks/Models/Buildings/RMainNeighborhoodModel.h \
    ./Networks/Models/Orchestrating/RNetworkManagerModel.h \
    ./Networks/Models/Orchestrating/RNetworkSamplingModel.h \
    ./Networks/Parameters/RPacket.h \
    ./Networks/Parameters/RPacketResponse.h \
    ./Networks/Parameters/RRequest.h \
    ./Networks/Parameters/RFinancingRequest.h \
    ./Networks/Parameters/RContractorNegotiationRequest.h \
    ./Networks/Parameters/REngineeringAssessmentRequest.h \
    ./Networks/Parameters/RSingleFamilyContractorRequest.h \
    ./Networks/Parameters/RMultiFamilyContractorRequest.h \
    ./Networks/Parameters/RPermitRequest.h \
    ./Networks/Estimators/RPopulationDisplacementEstimator.h \
    ./Networks/Estimators/RAHSPopulationDisplacementEstimator.h \
    ./Networks/Estimators/RAHSPreparednessEstimator.h \
    ./Networks/Estimators/RSutley2016MorbidityEstimator.h \
    ./Networks/Estimators/RAlisjahbana2021FinancingTimeEstimator.h \
    ./Networks/Estimators/RDisasterPreparednessEstimator.h \
    ./Networks/Models/Businesses/RSingleFamilyContractorFirmModel.h \
    ./Networks/Models/Businesses/RMultiFamilyContractorFirmModel.h \
    ./Networks/Models/Businesses/RContractorNegotiationModel.h \
    ./Networks/Models/Businesses/REngineeringFirmModel.h \
    ./Networks/Models/Businesses/RInspectorModel.h \
    ./Networks/Models/Businesses/RPermitAssessorModel.h \
    ./Networks/Models/Businesses/RPrivateMoneyLenderModel.h \
    ./Networks/Models/Businesses/RPublicMoneyLenderModel.h \
    ./Networks/Models/Businesses/RGrantProviderModel.h \
    ./Networks/Models/Businesses/RInsuranceCompanyModel.h \
    ./Networks/Models/Businesses/RGenericMaterialsSupplierModel.h \
    ./Networks/Models/Businesses/RHardwareStoreModel.h \
    ./Networks/Models/Businesses/RConcretePlantModel.h \


FORMS += \
    RMainWindow.ui \
    ./Core/RObjectWidget.ui \
    ./Core/RPropertyWidget.ui \
    ./Core/FlowchartWidget/RFlowchartWidget.ui \

INCLUDEPATH += \
    ./Core \
    ./Core/FlowchartWidget \
    ./Core/RDelegate \
    ./Hazards \
    ./Hazards/GroundMotionModels \
    ./Hazards/IntensityModels \
    ./Hazards/MagnitudeModels \
    ./Hazards/LocationModels \
    ./Hazards/Tools \
    ./Hazards/OccurrenceModels \
    ./SimpleBuildings \
    ./SimpleBuildings/Models \
    ./Networks \
    ./Networks/Parameters \
    ./Networks/Repositories \
    ./Networks/Estimators \
    ./Networks/Models \
    ./Networks/Models/Buildings \
         #./Networks/Models/Power \
         #./Networks/Models/Water \
         #./Networks/Models/Transportation \
         #./Networks/Models/Fuel \
         #./Networks/Tools \
    ./Networks/Models/Orchestrating \
    ./Networks/Models/Businesses \

#RESOURCES += \
#    Core/RIcon/R3.qrc



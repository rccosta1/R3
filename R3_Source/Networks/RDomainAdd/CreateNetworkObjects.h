// *******************************************************************************
// *******************************************************************************
//                                     PARAMETERS
// *******************************************************************************
// *******************************************************************************

factory.registerClass<RPacket>("PARAMETERS Packets");
factory.registerClass<RPacket>("RPacket");

factory.registerClass<RPacketResponse>("PARAMETERS Packet Responses");
factory.registerClass<RPacketResponse>("RPacketResponse");

factory.registerClass<RRequest>("PARAMETERS Requests");
factory.registerClass<RRequest>("RRequest");

factory.registerClass<RFinancingRequest>("PARAMETERS Requests");
factory.registerClass<RFinancingRequest>("RFinancingRequest");

factory.registerClass<REngineeringAssessmentRequest>("PARAMETERS Requests");
factory.registerClass<REngineeringAssessmentRequest>("REngineeringAssessmentRequest");

factory.registerClass<RPermitRequest>("PARAMETERS Requests");
factory.registerClass<RPermitRequest>("RPermitRequest");

factory.registerClass<RSingleFamilyContractorRequest>("PARAMETERS Requests");
factory.registerClass<RSingleFamilyContractorRequest>("RSingleFamilyContractorRequest");

factory.registerClass<RMultiFamilyContractorRequest>("PARAMETERS Requests");
factory.registerClass<RMultiFamilyContractorRequest>("RMultiFamilyContractorRequest");

factory.registerClass<RContractorNegotiationRequest>("PARAMETERS Requests");
factory.registerClass<RContractorNegotiationRequest>("RContractorNegotiationRequest");

// *******************************************************************************
// *******************************************************************************
//                                     REPOSITORIES
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RCensusBuildingPortfolio>("Information Repositories");
factory.registerClass<RCensusBuildingPortfolio>("RCensusBuildingPortfolio");

factory.registerClass<RHAZUSBuildingPortfolio>("Information Repositories");
factory.registerClass<RHAZUSBuildingPortfolio>("RHAZUSBuildingPortfolio");

factory.registerClass<RSimCenterBuildingPortfolio>("Information Repositories");
factory.registerClass<RSimCenterBuildingPortfolio>("RSimCenterBuildingPortfolio");

factory.registerClass<RHouseholdPortfolio>("Information Repositories");
factory.registerClass<RHouseholdPortfolio>("RHouseholdPortfolio");


// *******************************************************************************
// *******************************************************************************
//                                     ESTIMATORS
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RDisasterPreparednessEstimator>("Estimators");
factory.registerClass<RDisasterPreparednessEstimator>("RDisasterPreparednessEstimator");

factory.registerClass<RAHSPreparednessEstimator>("Estimators");
factory.registerClass<RAHSPreparednessEstimator>("RAHSPreparednessEstimator");

factory.registerClass<RStatisticsCanadaPreparednessEstimator>("Estimators");
factory.registerClass<RStatisticsCanadaPreparednessEstimator>("RStatisticsCanadaPreparednessEstimator");

factory.registerClass<RHAZUSPopulationDisplacementEstimator>("Estimators");
factory.registerClass<RHAZUSPopulationDisplacementEstimator>("RHAZUSPopulationDisplacementEstimator");

factory.registerClass<RAHSPopulationDisplacementEstimator>("Estimators");
factory.registerClass<RAHSPopulationDisplacementEstimator>("RAHSPopulationDisplacementEstimator");

factory.registerClass<RSutley2016MorbidityEstimator>("Estimators");
factory.registerClass<RSutley2016MorbidityEstimator>("RSutley2016MorbidityEstimator");


// *******************************************************************************
// *******************************************************************************
//                                     NETWORK MODELS
// *******************************************************************************
// *******************************************************************************
factory.registerClass<RPrivateMoneyLenderModel>("NETWORK MODELS Private Money Lender");
factory.registerClass<RPrivateMoneyLenderModel>("RPrivateMoneyLenderModel");

factory.registerClass<RBridgeModel>("NETWORK MODELS Bridges");
factory.registerClass<RBridgeModel>("RBridgeModel");

factory.registerClass<RContractorNegotiationModel>("NETWORK MODELS Contractor Firms");
factory.registerClass<RContractorNegotiationModel>("RContractorNegotiationModel");

factory.registerClass<RSingleFamilyContractorFirmModel>("NETWORK MODELS Home Contractor Firms");
factory.registerClass<RSingleFamilyContractorFirmModel>("RSingleFamilyContractorFirmModel");

factory.registerClass<RMultiFamilyContractorFirmModel>("NETWORK MODELS Building Contractor Firms");
factory.registerClass<RMultiFamilyContractorFirmModel>("RMultiFamilyContractorFirmModel");

factory.registerClass<RHardwareStoreModel>("NETWORK MODELS Material Suppliers");
factory.registerClass<RHardwareStoreModel>("RHardwareStoreModel");

factory.registerClass<RPublicMoneyLenderModel>("NETWORK MODELS Public Money Lender");
factory.registerClass<RPublicMoneyLenderModel>("RPublicMoneyLenderModel");

factory.registerClass<RGrantProviderModel>("NETWORK MODELS Grant Provider");
factory.registerClass<RGrantProviderModel>("RGrantProviderModel");

factory.registerClass<RCommunityModel>("NETWORK MODELS Communities");
factory.registerClass<RCommunityModel>("RCommunityModel");

factory.registerClass<RConcretePlantModel>("NETWORK MODELS Concrete Plants");
factory.registerClass<RConcretePlantModel>("RConcretePlantModel");

factory.registerClass<REngineeringFirmModel>("NETWORK MODELS Engineering Firms");
factory.registerClass<REngineeringFirmModel>("REngineeringFirmModel");

factory.registerClass<RFuelPipelineModel>("NETWORK MODELS Fuel Pipelines");
factory.registerClass<RFuelPipelineModel>("RFuelPipelineModel");

factory.registerClass<RFuelSourceModel>("NETWORK MODELS Fuel Sources");
factory.registerClass<RFuelSourceModel>("RFuelSourceModel");

factory.registerClass<RFuelTankFarmModel>("NETWORK MODELS Fuel Tank Farms");
factory.registerClass<RFuelTankFarmModel>("RFuelTankFarmModel");

factory.registerClass<RInspectorModel>("NETWORK MODELS Inspectors");
factory.registerClass<RInspectorModel>("RInspectorModel");

factory.registerClass<RNetworkManagerModel>("NETWORK MODELS Network Managers");
factory.registerClass<RNetworkManagerModel>("RNetworkManagerModel");

factory.registerClass<RPermitAssessorModel>("NETWORK MODELS Permitting Authority");
factory.registerClass<RPermitAssessorModel>("RPermitAssessorModel");

factory.registerClass<RPortModel>("NETWORK MODELS Ports");
factory.registerClass<RPortModel>("RPortModel");

factory.registerClass<RPowerLineModel>("NETWORK MODELS Power Lines");
factory.registerClass<RPowerLineModel>("RPowerLineModel");

factory.registerClass<RPowerDamModel>("NETWORK MODELS Power Sources");
factory.registerClass<RPowerDamModel>("RPowerDamModel");

factory.registerClass<RPowerSubstationModel>("NETWORK MODELS Power Substations");
factory.registerClass<RPowerSubstationModel>("RPowerSubstationModel");

factory.registerClass<RRailwayDispatchFacilityModel>("NETWORK MODELS Railway Dispatch Facilities");
factory.registerClass<RRailwayDispatchFacilityModel>("RRailwayDispatchFacilityModel");

factory.registerClass<RRailwayFuelFacilityModel>("NETWORK MODELS Railway Fuel Facilities");
factory.registerClass<RRailwayFuelFacilityModel>("RRailwayFuelFacilityModel");

factory.registerClass<RRefineryModel>("NETWORK MODELS Refineries");
factory.registerClass<RRefineryModel>("RRefineryModel");

factory.registerClass<RGenericFuelSourceModel>("NETWORK MODELS Refineries");
factory.registerClass<RGenericFuelSourceModel>("RGenericFuelSourceModel");

factory.registerClass<RGenericMaterialsSupplierModel>("NETWORK MODELS Hardware Stores");
factory.registerClass<RGenericMaterialsSupplierModel>("RGenericMaterialsSupplierModel");

factory.registerClass<RRoadwayModel>("NETWORK MODELS Roads");
factory.registerClass<RRoadwayModel>("RRoadwayModel");

factory.registerClass<RShippingChannelModel>("NETWORK MODELS Shipping Channel");
factory.registerClass<RShippingChannelModel>("RShippingChannelModel");

factory.registerClass<RWaterPumpStationModel>("NETWORK MODELS Water Lifting Stations");
factory.registerClass<RWaterPumpStationModel>("RWaterPumpStationModel");

factory.registerClass<RWaterPipelineModel>("NETWORK MODELS Water Pipelines");
factory.registerClass<RWaterPipelineModel>("RWaterPipelineModel");

factory.registerClass<RWaterDamModel>("NETWORK MODELS Water Reservoirs");
factory.registerClass<RWaterDamModel>("RWaterDamModel");

factory.registerClass<RWaterReservoirModel>("NETWORK MODELS Water Reservoirs");
factory.registerClass<RWaterReservoirModel>("RWaterReservoirModel");

factory.registerClass<RWaterStorageTankModel>("NETWORK MODELS Water Storage Tanks");
factory.registerClass<RWaterStorageTankModel>("RWaterStorageTankModel");

factory.registerClass<RWaterTreatmentFacilityModel>("NETWORK MODELS Water Treatment Facilities");
factory.registerClass<RWaterTreatmentFacilityModel>("RWaterTreatmentFacilityModel");

factory.registerClass<RWaterDemandZoneModel>("NETWORK MODELS Water Demand Zones");
factory.registerClass<RWaterDemandZoneModel>("RWaterDemandZoneModel");

factory.registerClass<RNetworkSamplingModel>("NETWORK MODELS Sampling Models");
factory.registerClass<RNetworkSamplingModel>("RNetworkSamplingModel");

factory.registerClass<RResidentialMobilityNeighborhoodModel>("NETWORK MODELS Neighborhoods");
factory.registerClass<RResidentialMobilityNeighborhoodModel>("RResidentialMobilityNeighborhoodModel");

factory.registerClass<RMainNeighborhoodModel>("NETWORK MODELS Neighborhoods");
factory.registerClass<RMainNeighborhoodModel>("RMainNeighborhoodModel");

factory.registerClass<RWorkforceStudyNeighborhoodModel>("NETWORK MODELS Neighborhoods");
factory.registerClass<RWorkforceStudyNeighborhoodModel>("RWorkforceStudyNeighborhoodModel");

factory.registerClass<RRecoveryStrategyStudyNeighborhoodModel>("NETWORK MODELS Neighborhoods");
factory.registerClass<RRecoveryStrategyStudyNeighborhoodModel>("RRecoveryStrategyStudyNeighborhoodModel");

factory.registerClass<RInsuranceCompanyModel>("NETWORK MODELS Insurance Companies");
factory.registerClass<RInsuranceCompanyModel>("RInsuranceCompanyModel");


// *******************************************************************************
// *******************************************************************************
//                                     METHODS
// *******************************************************************************
// *******************************************************************************


// *******************************************************************************
// *******************************************************************************
//                                     OTHER
// *******************************************************************************
// *******************************************************************************



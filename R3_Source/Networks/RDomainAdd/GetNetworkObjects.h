else if (className == QString("MODELS Network Motion").remove(' ')) {
            QList<RNetworkMotionModel *> objectList = this->findChildren<RNetworkMotionModel *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }
        else if (className == QString("MODELS Refineries").remove(' ')) {
            QList<RRefineryModel *> objectList = this->findChildren<RRefineryModel *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }

        else if (className == QString("MODELS TankFarms").remove(' ')) {
            QList<RTankFarmModel *> objectList = this->findChildren<RTankFarmModel *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }
        else if (className == QString("MODELS Communities").remove(' ')) {
            QList<RCommunityModel *> objectList = this->findChildren<RCommunityModel *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }
        else if (className == QString("MODELS Ports").remove(' ')) {
            QList<RPortModel *> objectList = this->findChildren<RPortModel *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }
        else if (className == QString("MODELS Bridges").remove(' ')) {
            QList<RBridgeModel *> objectList = this->findChildren<RBridgeModel *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }
        else if (className == QString("PARAMETERS Packets").remove(' ')) {
            QList<RPacket *> objectList = this->findChildren<RPacket *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }
        else if (className == QString("PARAMETERS Packet Responses").remove(' ')) {
            QList<RPacketResponse *> objectList = this->findChildren<RPacketResponse *>();
            for (int i = 0; i < objectList.count(); i++) {
                result << objectList[i];
            }
        }


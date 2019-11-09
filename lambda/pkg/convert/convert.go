package convert

import (
	"encoding/json"
	"io"
	"time"

	"github.com/franpog859/waterinator/lambda/pkg/model"
)

func ExtractSensorDataFromBody(body io.ReadCloser) (model.SensorDataEmbedded, error) {
	decoder := json.NewDecoder(body)
	var sensorDataEmbedded model.SensorDataEmbedded
	err := decoder.Decode(&sensorDataEmbedded)
	if err != nil {
		return model.SensorDataEmbedded{}, err
	}
	return sensorDataEmbedded, nil
}

func ConvertToSensorDataDatabase(sensorDataInput model.SensorDataEmbedded) model.SensorDataDatabase {
	didWater := sensorDataInput.DidWater != 0
	waterTime := time.Now().Format(model.TimeLayout)

	return model.SensorDataDatabase{
		HardwareID:      sensorDataInput.HardwareID,
		HumidityPercent: sensorDataInput.HumidityPercent,
		DidWater:        didWater,
		Time:            waterTime,
	}
}

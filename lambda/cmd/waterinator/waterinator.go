package waterinator

import (
	"log"
	"net/http"

	"github.com/franpog859/waterinator/lambda/pkg/convert"
	"github.com/franpog859/waterinator/lambda/pkg/database"
	"github.com/pkg/errors"
)

// SaveSensorDataToDatabase saves sensor data to database
func SaveSensorDataToDatabase(w http.ResponseWriter, r *http.Request) {
	sensorDataInput, err := convert.ExtractSensorDataFromBody(r.Body)
	if err != nil {
		w.WriteHeader(http.StatusBadRequest)
		err = errors.Wrap(err, "failed to extract sensor data")
		w.Write([]byte(err.Error()))
		log.Println(err)
		return
	}

	client, err := database.NewClient()
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		err = errors.Wrap(err, "failed to create database client")
		w.Write([]byte(err.Error()))
		log.Println(err)
		return
	}

	sensorDataOutput := convert.ConvertToSensorDataDatabase(sensorDataInput)

	err = client.PostSensorData(sensorDataOutput)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		err = errors.Wrap(err, "failed to post data")
		w.Write([]byte(err.Error()))
		log.Println(err)
		return
	}

	w.WriteHeader(http.StatusOK)
}

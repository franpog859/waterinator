package waterinator

import (
	"log"
	"net/http"

	"github.com/franpog859/waterinator/lambda/internal/database"
	"github.com/pkg/errors"
)

func SaveSensorDataToDatabase(w http.ResponseWriter, r *http.Request) {
	//TODO: Check if sensor data is correct

	client, err := database.NewClient()
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		err = errors.Wrap(err, "failed to create database client")
		w.Write([]byte(err.Error()))
		log.Println(err)
		return
	}

	err = client.PostSensorData("some data")
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		err = errors.Wrap(err, "failed to post data")
		w.Write([]byte(err.Error()))
		log.Println(err)
		return
	}

	w.WriteHeader(http.StatusOK)
}

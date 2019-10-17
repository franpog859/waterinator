package database

import (
	"context"
	"fmt"
	"os"

	"github.com/franpog859/waterinator/lambda/pkg/model"
	"github.com/pkg/errors"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

const (
	mongoDBServiceURIFormat = "mongodb+srv://waterinator:%s@franpog-cluster-uteiq.mongodb.net/test?retryWrites=true&w=majority"
	databaseName            = "waterinator"
	collectionName          = "sensordata"
	passwordEnvName         = "DATABASE_PASSWORD"
)

// Client interface
type Client interface {
	PostSensorData(string) error
}

type client struct {
	collection *mongo.Collection
}

// NewClient provides Client interface
func NewClient() (Client, error) {
	mongoDBServiceURI, err := provideFullMongoDBServiceURI()
	if err != nil {
		return nil, errors.Wrap(err, "failed to provide full mongo service URI")
	}

	clientOptions := options.Client().ApplyURI(mongoDBServiceURI)
	mongoDBClient, err := mongo.Connect(context.TODO(), clientOptions)
	if err != nil {
		return nil, fmt.Errorf("failed to connect MongoDB client: %v", err)
	}

	collection := mongoDBClient.Database(databaseName).Collection(collectionName)

	return &client{
		collection: collection,
	}, nil
}

func (c *client) PostSensorData(data string) error {
	sensorData := model.SensorData{
		Data: data,
	}

	_, err := c.collection.InsertOne(context.TODO(), sensorData)
	if err != nil {
		return errors.Wrap(err, "failed to insert sensor data")
	}

	return nil
}

func provideFullMongoDBServiceURI() (string, error) {
	password := os.Getenv(passwordEnvName)
	if password == "" {
		return "", fmt.Errorf("failed to read password from environment variable: %s", passwordEnvName)
	}

	uri := fmt.Sprintf(mongoDBServiceURIFormat, password)
	return uri, nil
}

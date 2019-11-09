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
	uriFormatEnvName        = "MONGO_URI_FORMAT"
	databaseNameEnvName     = "DATABASE_NAME"
	databaseUserNameEnvName = "DATABASE_USERNAME"
	passwordEnvName         = "DATABASE_PASSWORD"
	collectionNameEnvName   = "COLLECTION_NAME"
)

// Client interface
type Client interface {
	PostSensorData(model.SensorDataDatabase) error
}

type client struct {
	collection *mongo.Collection
}

type initData struct {
	mongoDBServiceURI string
	databaseName      string
	collectionName    string
}

// NewClient provides Client interface
func NewClient() (Client, error) {
	init, err := getInitData()
	if err != nil {
		return nil, errors.Wrap(err, "failed to get init data")
	}

	clientOptions := options.Client().ApplyURI(init.mongoDBServiceURI)
	mongoDBClient, err := mongo.Connect(context.TODO(), clientOptions)
	if err != nil {
		return nil, fmt.Errorf("failed to connect MongoDB client: %v", err)
	}

	collection := mongoDBClient.Database(init.databaseName).Collection(init.collectionName)

	return &client{
		collection: collection,
	}, nil
}

func (c *client) PostSensorData(sensorData model.SensorDataDatabase) error {
	_, err := c.collection.InsertOne(context.TODO(), sensorData)
	if err != nil {
		return errors.Wrap(err, "failed to insert sensor data")
	}

	return nil
}

func getInitData() (initData, error) {
	mongoDBServiceURIFormat, err := getEnv(uriFormatEnvName)
	if err != nil {
		return initData{}, err
	}
	username, err := getEnv(databaseUserNameEnvName)
	if err != nil {
		return initData{}, err
	}
	password, err := getEnv(passwordEnvName)
	if err != nil {
		return initData{}, err
	}
	databaseName, err := getEnv(databaseNameEnvName)
	if err != nil {
		return initData{}, err
	}
	collectionName, err := getEnv(collectionNameEnvName)
	if err != nil {
		return initData{}, err
	}
	return initData{
		mongoDBServiceURI: fmt.Sprintf(mongoDBServiceURIFormat, username, password),
		databaseName:      databaseName,
		collectionName:    collectionName,
	}, nil
}

func getEnv(envName string) (string, error) {
	env := os.Getenv(envName)
	if env == "" {
		return "", fmt.Errorf("failed to read environment variable: %s", envName)
	}
	return env, nil
}

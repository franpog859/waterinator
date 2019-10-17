package database

import (
	"context"
	"fmt"

	"github.com/franpog859/waterinator/lambda/internal/model"
	"github.com/pkg/errors"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

const (
	mongoDBServiceURI = "mongodb://mongo-database-internal:27017"
	databaseName      = "waterinator"
	collectionName    = "sensordata"
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

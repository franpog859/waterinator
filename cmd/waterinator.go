package main

import (
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)

const (
	portEnvName  = "PORT"
	localDevPort = "8000"
)

func main() {
	gin.SetMode(gin.ReleaseMode)

	externalRouter := gin.Default()
	externalRouter.GET("/", func(context *gin.Context) {
		context.JSON(http.StatusOK, "Hello!")
	})

	port := providePort()
	externalRouter.Run(port)
}

func providePort() string {
	port := os.Getenv(portEnvName)
	if port == "" {
		port = localDevPort
	}
	port = ":" + port
	return port
}

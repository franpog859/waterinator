package main

import (
	"log"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)

const (
	externalPort = ":8000"
)

func main() {
	externalPort := os.Getenv("PORT")

	if externalPort == "" {
		log.Fatal("$PORT must be set")
	}

	gin.SetMode(gin.ReleaseMode)

	externalRouter := gin.Default()
	externalRouter.GET("/", func(context *gin.Context) {
		context.JSON(http.StatusOK, "Hello!")
	})

	externalRouter.Run(externalPort)
}

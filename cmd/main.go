package main

import (
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)

const (
	externalPort = "8000"
)

func main() {
	port := os.Getenv("PORT")
	if port == "" {
		port = externalPort
	}

	gin.SetMode(gin.ReleaseMode)

	externalRouter := gin.Default()
	externalRouter.GET("/", func(context *gin.Context) {
		context.JSON(http.StatusOK, "Hello!")
	})

	externalRouter.Run(":" + port)
}

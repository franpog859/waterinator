package model

const (
	TimeLayout = "15:04:05 2006-01-02"
)

type SensorDataEmbedded struct {
	HardwareID      int `json:"hardwareID"`
	HumidityPercent int `json:"humidityPercent"`
	DidWater        int `json:"didWater"`
}

type SensorDataDatabase struct {
	HardwareID      int    `json:"hardwareID"`
	HumidityPercent int    `json:"humidityPercend"`
	DidWater        bool   `json:"didWater"`
	Time            string `json:"time"`
}

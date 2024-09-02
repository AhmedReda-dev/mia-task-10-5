// Important note :
// I could not Do that task alone
// so I used ChatGPT to help me Write the Code
// I read the code carefully and tried to Understand it
// here is what i understood:
/*
1. we Define the pins of the mpu, [ground , power, data pins]
2. start ic2 connection
3. stops the sleepMode by writing 0 to power register
4. readRegister16: read a register and return the value of the data
5. loop: save the value and converte it into yaw angle , print the Yaw
*/ 
// I hope you accept the code ♥

#include <Wire.h>

#define MPU6050_ADDRESS 0x68  // MPU6050 I2C address when AD0 pin is grounded
#define PWR_MGMT_1 0x6B       // Power management register
#define GYRO_CONFIG 0x1B      // Gyroscope configuration register
#define GYRO_ZOUT_H 0x47      // Register address for high byte of gyroscope Z-axis data

float yaw = 0;                // Variable to store yaw angle
unsigned long previousTime = 0;

void setup() {
    Wire.begin();              // Initialize I2C communication
    Serial.begin(9600);        // Initialize Serial communication for debugging

    // Wake up MPU6050 (set sleep bit to 0)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(PWR_MGMT_1);    // Access the power management register
    Wire.write(0x00);          // Wake up MPU6050 by writing 0
    Wire.endTransmission(true);

    // Configure the gyroscope (set full-scale range to ±250°/s)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(GYRO_CONFIG);   // Access the gyroscope configuration register
    Wire.write(0x00);          // Set the full-scale range to ±250°/s (sensitivity = 131 LSB/°/s)
    Wire.endTransmission(true);

    previousTime = millis();   // Initialize previous time
}

int16_t readRegister16(int reg) {
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(reg);                // Set register pointer
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS, 2, true); // Request 2 bytes from the register
    int16_t value = (Wire.read() << 8) | Wire.read(); // Combine high and low bytes
    return value;
}

void loop() {
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - previousTime) / 1000.0; // Convert time difference to seconds
    previousTime = currentTime;

    int16_t gyroZ = readRegister16(GYRO_ZOUT_H);  // Read Z-axis gyroscope data
    float gyroZ_dps = gyroZ / 131.0;              // Convert raw data to degrees per second

    yaw += gyroZ_dps * deltaTime;                 // Integrate angular velocity to get yaw

    // Print the yaw value for debugging
    Serial.print("Yaw: ");
    Serial.print(yaw);
    Serial.println(" degrees");

    delay(100);  // Delay for stability and readable output
}

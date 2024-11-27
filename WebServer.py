from flask import Flask
import RPi.GPIO as GPIO
import time

app = Flask(__name__)

# Configuração do GPIO para o Servo Motor
servo_pin = 18  # Substitua pelo pino GPIO conectado ao servo
GPIO.setmode(GPIO.BCM)
GPIO.setup(servo_pin, GPIO.OUT)
pwm = GPIO.PWM(servo_pin, 50)  # Frequência de 50Hz para servo motor
pwm.start(0)

def rotate_servo(angle):
    """Função para girar o servo para o ângulo desejado"""
    duty_cycle = 2 + (angle / 18)  # Converte ângulo para ciclo de trabalho
    pwm.ChangeDutyCycle(duty_cycle)
    time.sleep(0.5)
    pwm.ChangeDutyCycle(0)  # Para o sinal PWM após o movimento

@app.route("/")
def index():
    """Página inicial com um botão para girar o motor"""
    return """
    <h1>Controle de Servo com Raspberry Pi</h1>
    <button onclick="fetch('/rotate')">Girar 100 Graus</button>
    <script>
      // Adiciona feedback visual ao pressionar o botão
      document.querySelector("button").addEventListener("click", function() {
        alert("Comando enviado para girar 100 graus!");
      });
    </script>
    """

@app.route("/rotate")
def rotate():
    """Rota para girar o servo motor"""
    rotate_servo(100)  # Gira o servo para 100 graus
    return "Servo girado 100 graus!"

if __name__ == "__main__":
    try:
        app.run(host="0.0.0.0", port=5000)  # Servidor disponível na rede local
    finally:
        pwm.stop()
        GPIO.cleanup()

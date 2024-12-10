const mongoose = require('mongoose');
const Gpio = require('onoff').Gpio; // Biblioteca para controle de GPIO

// Configuração do esquema da fechadura
const fechaduraSchema = new mongoose.Schema({
    numeroFechadura: {
        type: String,
        required: true,
        unique: true
    },
    senha: {
        type: String,
        required: true,
    }
});

// Configuração do motor (GPIO pinos da Raspberry Pi)
const motor1 = new Gpio(17, 'out'); // Pino GPIO 17
const motor2 = new Gpio(18, 'out'); // Pino GPIO 18

// Função para girar o motor para abrir a fechadura
fechaduraSchema.methods.abrir = function () {
    console.log(`Abrindo fechadura ${this.numeroFechadura}...`);
    motor1.writeSync(1); // Ativa o pino 17
    motor2.writeSync(0); // Desativa o pino 18
    setTimeout(() => pararMotor(), 5000); // Gira por 5 segundos e para
};

// Função para girar o motor para fechar a fechadura
fechaduraSchema.methods.fechar = function () {
    console.log(`Fechando fechadura ${this.numeroFechadura}...`);
    motor1.writeSync(0); // Desativa o pino 17
    motor2.writeSync(1); // Ativa o pino 18
    setTimeout(() => pararMotor(), 5000); // Gira por 5 segundos e para
};

// Função para parar o motor
function pararMotor() {
    console.log('Parando o motor...');
    motor1.writeSync(0); // Desativa o pino 17
    motor2.writeSync(0); // Desativa o pino 18
}

// Model da fechadura
const Fechadura = mongoose.model('Fechadura', fechaduraSchema);

// Exporta o modelo
module.exports = Fechadura;


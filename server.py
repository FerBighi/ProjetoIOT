from flask import Flask, request, jsonify
from datetime import datetime

app = Flask(__name__)

@app.route('/api/telemetria', methods=['POST'])
def receber_telemetria():
    # Verifica se a requisição contém um JSON válido
    if not request.is_json:
        return jsonify({"erro": "O cabeçalho Content-Type deve ser application/json"}), 400
    
    dados = request.get_json()
    
    # Extrai os dados do payload
    dispositivo_id = dados.get('dispositivo_id')
    mac_address = dados.get('mac_address')
    temperatura = dados.get('temperatura_local')
    umidade = dados.get('umidade_local')
    
    # Formata e exibe os dados no console do servidor
    print(f"\n--- [Nova Telemetria Recebida: {datetime.now().strftime('%H:%M:%S')}] ---")
    print(f"ID do Dispositivo : {dispositivo_id}")
    print(f"Endereço MAC      : {mac_address}")
    print(f"Temperatura       : {temperatura} °C")
    print(f"Umidade           : {umidade} %")
    print("-" * 50)
    
    # Resposta de confirmação para o ESP32
    resposta = {
        "status": "sucesso",
        "mensagem": "Dados de telemetria processados com sucesso",
        "timestamp": datetime.now().isoformat()
    }
    return jsonify(resposta), 201

if __name__ == '__main__':
    # Roda o servidor acessível para qualquer dispositivo na mesma rede local
    app.run(host='0.0.0.0', port=5000, debug=True)

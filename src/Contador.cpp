#include "Contador.h"
#include <chrono>

Contador::Contador()
    : m_valor(0),
    m_intervalo_ms(1000),
    m_incremento(1),
    m_rodando(false) {
}

Contador::~Contador() {
    // Garante que a thread termine ao destruir o objeto
    stop();

    if (m_thread_contador.joinable()) {
        m_thread_contador.join();
    }
}

void Contador::start() {
	// Evita iniciar múltiplas threads
	if (m_rodando) return;

	//Seta a flag de execução
	m_rodando = true;
    
	//Aguarda a finalização da thread anterior, se houver
	if (m_thread_contador.joinable()) {
        m_thread_contador.join();
    }

	//Inicia a thread de contagem
    m_thread_contador = std::thread(&Contador::loopContador, this);
}

// Para a contagem
void Contador::stop() {
    m_rodando = false;
}

// Reseta o valor do contador para zero e notifica os observadores
void Contador::reset() {
    m_valor = 0;
    notificarObservadores();
}

// Define o intervalo de tempo entre incrementos (em milissegundos)
void Contador::setIntervalo(int ms) {
    m_intervalo_ms = (ms > 0) ? ms : 1000;
}

void Contador::setIncremento(int valor) {
    m_incremento = valor;
}

// Função executada na thread para incrementar o contador
void Contador::loopContador() {
    while (m_rodando) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_intervalo_ms.load()));
        if (!m_rodando) break; // Verifica novamente após a pausa

        m_valor += m_incremento.load();
        notificarObservadores();
    }
}

//Adiciona o observador
void Contador::adicionarObservador(ContadorObservador* obs) {
    std::lock_guard<std::mutex> lock(m_mutex_observadores);
    m_observadores.push_back(obs);
}

//Remove o observador
void Contador::removerObservador(ContadorObservador* obs) {
    std::lock_guard<std::mutex> lock(m_mutex_observadores);
}

// Notifica todos os observadores sobre a atualização do valor
void Contador::notificarObservadores() {
    std::lock_guard<std::mutex> lock(m_mutex_observadores);
    for (ContadorObservador* obs : m_observadores) {
        if (obs) {
            obs->onContadorAtualizado(m_valor.load());
        }
    }
}
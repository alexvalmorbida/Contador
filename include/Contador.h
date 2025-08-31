#ifndef CONTADOR_H
#define CONTADOR_H

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

/**
 * @class ContadorObservador
 * @brief Interface para quem quiser "ouvir" as atualizações do Contador.
 */
class ContadorObservador {
public:
    virtual ~ContadorObservador() = default;
    // Método que será chamado quando o valor do contador for atualizado.
    virtual void onContadorAtualizado(long long novoValor) = 0;
};

/**
 * @class Contador
 * @brief Gerencia a lógica de contagem em uma thread separada.
 */
class Contador {
public:
    Contador();
    ~Contador();

	void start();   // Inicia a contagem em uma thread separada
	void stop();    // Para a contagem
	void reset();   // Reseta o valor do contador para zero

	void setIntervalo(int ms);      // Define o intervalo de atualização em milissegundos
	void setIncremento(int valor);  // Define o valor do incremento

    void adicionarObservador(ContadorObservador* obs);
    void removerObservador(ContadorObservador* obs);

private:
    void loopContador();            // A função que será executada na thread
    void notificarObservadores();

	std::atomic<long long> m_valor;     // Valor atual do contador
	std::atomic<int> m_intervalo_ms;    // Intervalo de atualização em milissegundos
	std::atomic<int> m_incremento;      // Valor do incremento
	std::atomic<bool> m_rodando;        // Flag para controlar a execução da thread

	std::thread m_thread_contador;      // Thread que executa a contagem
	std::vector<ContadorObservador*> m_observadores;    // Lista de observadores
    std::mutex m_mutex_observadores; // Protege o acesso ao vetor de observadores
};

#endif
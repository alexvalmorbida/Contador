#ifndef JANELA_CONTADOR_H
#define JANELA_CONTADOR_H

#include <gtkmm.h>
#include "Contador.h"

class JanelaContador : public Gtk::Window, public ContadorObservador {
public:
    JanelaContador();
    virtual ~JanelaContador();

protected:
	void onContadorAtualizado(long long novoValor) override; // Atualiza o valor exibido na UI

private:
	Contador m_contador;						// Instância do contador

    // Callbacks dos widgets da interface gráfica
	void on_start_stop_clicked();				// Inicia ou para a contagem
	void on_reset_clicked();					// Reseta o contador

    // Funções de auxiliares
	void notificarAtualizacaoUI();				// Notifica a thread principal para atualizar a UI
	void inicializar_estilos();					// Inicializa estilos CSS para os widgets

    // Estado para comunicação entre threads
	std::atomic<long long> m_valor_recebido;    // Valor recebido do contador
	Glib::Dispatcher m_dispatcher;              // Dispatcher para comunicação entre threads

    // Widgets da Interface Gráfica
	Gtk::Box m_vbox;							// Container principal vertical
	Gtk::Frame m_frame_contador;				// Frame para o contador
	Gtk::Label m_label_contador;				// Label que exibe o valor do contador
	Gtk::Button m_button_start_stop;			// Botão para iniciar/parar a contagem
	Gtk::Button m_button_reset;					// Botão para resetar o contador
	Gtk::Entry m_entry_intervalo;				// Entrada para o intervalo de atualização
	Gtk::Entry m_entry_incremento;				// Entrada para o valor do incremento
	Gtk::Box m_hbox_botoes;						// Container horizontal para os botões
	Gtk::Box m_hbox_entradas;					// Container horizontal para as entradas
};
#endif

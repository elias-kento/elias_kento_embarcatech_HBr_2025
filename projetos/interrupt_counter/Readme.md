## 🧠 Explicação da Lógica da Tarefa

Desenvolva um programa, em linguagem C, que implemente um **contador decrescente controlado por interrupção**, com o seguinte comportamento:

### 📲 Funcionamento do Botão A (GPIO5)

- Toda vez que o **Botão A** for pressionado:
  - O contador decrescente reinicia em **9**;
  - O valor do contador é mostrado no **display OLED**;
  - O sistema entra em modo de **contagem regressiva ativa**, decrementando de 1 em 1 a cada segundo, até chegar em **zero**.

### 🖲 Funcionamento do Botão B (GPIO6)

- Durante a contagem (de **9 até 0**):
  - O programa deve **registrar quantas vezes o Botão B foi pressionado**;
  - A quantidade de cliques deve ser **mostrada no OLED em tempo real**;
- Quando o contador atingir **zero**:
  - O sistema congela;
  - Cliques no botão B **são ignorados** (não devem ser acumulados);
  - O display permanece exibindo:
    - O valor **0** no contador;
    - A quantidade final de **cliques no Botão B** durante os 9 segundos de contagem.

- A contagem **só é reiniciada** quando o Botão A for pressionado novamente:
  - O contador volta para **9**;
  - O número de cliques do Botão B é **zerado**;
  - A contagem recomeça.

---

## ✅ Requisitos de Implementação

- Utilize **interrupções** para detectar os cliques nos botões **A** e **B**;
- A lógica da contagem decrescente e atualização do display OLED deve ocorrer no **loop principal** (via temporizador);
- A variável que armazena os cliques do botão B deve ser **zerada sempre que a contagem regressiva reiniciar**;
- O OLED deve mostrar, em tempo real:
  - O **valor atual do contador** (de 9 a 0);
  - A **quantidade de cliques no Botão B** dentro da contagem em andamento.

---

## 📤 Entrega

Siga as boas práticas ao carregar o seu projeto no GitHub.

📎 **Responda à tarefa enviando apenas o link do repositório GitHub contendo o projeto.**

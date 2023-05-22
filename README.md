# Side Scrolling Game

Utilizando os conceitos de transformações geométricas, visualização e modelagem, modelagem hierárquica e animação bidimensional foi implementada uma versão
simplificada de um jogo side scrolling.

## To do
- [X] Tela de fundo consiste em um cenário fixo que é visualizado parcialmente, a cada instante do jogo, conforme a
movimentação do personagem principal;
- [X] O cenário deve ser construídos a partir de primitivas como: quadriláteros, triângulos e círculos;
- [X] A tela deve ser visualizada em loop. A transição do final para o início deve ser imperceptível;
- [X] Personagem deve caminhar da esquerda para a direita;
- [X] Personagem construído a partir de: quadriláteros, círculo e triângulo, centrados na origem;
- [X] Personagem saltar ao acionarmos uma tecla do teclado. (Tecla j)
- [X] A cada vez que o personagem volta ao início da tela, os elementos de bonificação devem ser (aleatoriamente) adicionados à paisagem de forma apropriada;
- [X] Quando o personagem captura o elemento, o elemento deixa de aparecer na tela;
- [X] Crie um enredo que dê utilidade para os elementos de bonificação criados. Os elementos podem atribuir poder (um salto mais alto por exemplo),
energia ou vidas extras.
- [X] Crie um enredo que justifique o elemento criado. Ao tocar no elemento, o personagem perde uma vida, perde energia ou perde um poder adquirido.
- [ ] Utilizar mais orientação a objetos
- [ ] Refatorar Código
- [ ] Melhorar Descida Salto
- [ ] Colocar Tela Inicial
- [ ] Criar mais objetos
- [ ] Fazer transição dia/Noite

## Organização trabalho
- Para cada item que será criada uma branch com a feature a ser implementada.
- Ao final da criação da feature testar e mergear com a master.

## Teclas Jogo
- [ s ou S ] : Iniciar o jogo ou sair do buraco
- [ j ou J ] : Pular

## Instalação e execução
- **Mac**
```
g++ -o game main.cpp -framework OpenGL -framework GLUT
```
```
./game
```


- *Linux*
```
g++ -o game main.cpp -lglut -lGLU -lGL
```
```
./game
```

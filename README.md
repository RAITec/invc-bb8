
<p align="center" width="100%">
    <img width="33%" src="https://github.com/user-attachments/assets/325bb056-378b-4c98-8ddc-74a97417cbd5">
</p>

# 🔧 O que é
Essa é uma área separada para realizar testes em uma área isolada do código/projeto principal, com o objetivo de facilitar e tornar mais dinâmica essa etapa.


# ✏️ Por quê é importante

Não seria ideal fazer mudanças no código com o propósito de consertar um problema e se embananar para retornar à versão original para tentar algo diferente. Com essa organização teremos um controle melhor de versões de testes, além de diferentes tipos de testes em uma ala separada do projeto principal.

Ou seja, esse branch oferece:
- **Flexibilidade**
- **Autonomia**
- **Segurança**
- **Controle**

#  🫡 Como trabalhar

- Quando for realizar um testes ou resolver algum enventual problema que surigr no código crie um **branch** a partir deste, por exemplo, o bb8 não está tocando música interagindo com o ambiente, então será feito um branch de nome *interação ambiente* para resolver esse problema:
```
git checkout testes
git checkout -b interacao_ambiente #esse é só o exemplo, a partir de agora vou usar [nome]
```
- Após resolver o problema faça essa sequência de comandos para adicionar ao branch testes:
> Juntar os branches em um só e salvar as mudanças

```
git checkout testes
git pull origin master
git add .
git merge --squash [nome] #Isso vai juntar todos os commits em um
git commit -m "mensagem breve explicando as mudanças"
```

> Colocando as mudanças no site(remoto) e apagando o branch de bug fix
```
git push -u origin testes
git push -d origin [nome]
```

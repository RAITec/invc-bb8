<p align="center" width="100%">
    <img width="100%" src="https://github.com/user-attachments/assets/750bfef7-704e-40ed-bb14-4ced8bec4449">
</p>

# 😶‍🌫️Configurando o git

## SSH
Antes de absolutamente tudo é necessário colocar uma chave ssh no seu perfil para trabalhar localmente em um repositório
### 🪟 Windows
- Abra Git Bash
```
$ ls -al ~/.ssh
# Lista os arquivos no seu diretório .ssh, se existirem
```
- Cole o texto abaixo, substituindo o email usado no exemplo pelo seu endereço de email do GitHub.
```
ssh-keygen -t ed25519 -C "seu_emaill@exemplo.com"
```
- Isto cria uma nova chave SSH, usando o nome de e-mail fornecido como uma etiqueta. Simplismente aperte **Enter** em tudo que ele pedir

#### Adicionar sua chave SSH ao ssh-agent

- Em uma nova janela do PowerShell _com privilégios elevados de administrador_
```powershell
Get-Service -Name ssh-agent | Set-Service -StartupType Manual
Start-Service ssh-agent
```

- Em uma janela de terminal _sem permissões elevadas_, adicione sua chave privada SSH ao agente ssh.
```
ssh-add c:/Users/YOU/.ssh/id_ed25519
```

#### Colocar a chave no perfil

```
$ clip < ~/.ssh/id_ed25519.pub
# Copia os conteúdos de id_ed25519.pub para o clipboard
```

- No canto superior direito de qualquer página do GitHub, clique sua foto de perfil e, em seguida, clique em Configurações.
- Na seção "Acesso" da barra lateral, clique em Chaves SSH e GPG.
- Clique em Nova chave SSH ou Adicionar chave SSH.
- No campo "Title" (Título), adicione uma etiqueta descritiva para a nova chave. Por exemplo, se estiver usando um laptop pessoal, você poderá chamar essa chave de "Laptop pessoal".
- Selecione o tipo de chave: autenticação ou assinatura
- No campo "Chave", cole sua chave pública.
- Clique em Adicionar chave SSH. 

### 🐧 Linux


- Abra o terminal
```
$ ls -al ~/.ssh
# Lista os arquivos no seu diretório .ssh, se existirem
```
- Cole o texto abaixo, substituindo o email usado no exemplo pelo seu endereço de email do GitHub.
```
ssh-keygen -t ed25519 -C "seu_emaill@exemplo.com"
```
- Isto cria uma nova chave SSH, usando o nome de e-mail fornecido como uma etiqueta. Simplismente aperte **Enter** em tudo que ele pedir

#### Adicionar sua chave SSH ao ssh-agent
```
$ eval "$(ssh-agent -s)"
```
- Adicione sua chave SSH privada ao ssh-agent.
```
ssh-add ~/.ssh/id_ed25519
```

#### Colocar a chave no perfil

```
$ cat ~/.ssh/id_ed25519.pub
# Selecione e copie todos os conteúdos do arquivo
```

- No canto superior direito de qualquer página do GitHub, clique sua foto de perfil e, em seguida, clique em Configurações.
- Na seção "Acesso" da barra lateral, clique em Chaves SSH e GPG.
- Clique em Nova chave SSH ou Adicionar chave SSH.
- No campo "Title" (Título), adicione uma etiqueta descritiva para a nova chave. Por exemplo, se estiver usando um laptop pessoal, você poderá chamar essa chave de "Laptop pessoal".
- Selecione o tipo de chave: autenticação ou assinatura
- No campo "Chave", cole sua chave pública.
- Clique em Adicionar chave SSH. 

## Como começar a trabalhar no projeto
Copiando desse repositório na sua máquina:
```
git clone https://github.com/RAITec/invc-bb8.git
```
A partir disso você vai conseguir mudar o que quiser do repositório localmente, mas se for trabalhar em alguma coisa sempre o faça no branch de testes:
```
git checkout testes
```

Para que você faça o link entre o repositório local e o remoto(github) faça esse comando:
```
git remote add origin https://github.com/RAITec/invc-bb8.git
```

Pronto 😐👍 agora você pode trabaia

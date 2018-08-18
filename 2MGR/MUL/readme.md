## Projekt do předmětu MUL - Databáze videozáznamů

#### Hodnoceni: 29/29

Lukáš Pelánek <xpelan03@stud.fit.vutbr.cz>

## Požadavky
* PHP >= 7.1
* NPM >= 5.3
* OpenSSL PHP Extension
* PDO PHP Extension
* Mbstring PHP Extension
* Tokenizer PHP Extension
* Fileinfo PHP Extension
* DB
* mod_rewrite

## Instalace

Pro instalaci postupujte dle následujících kroků.

##### Composer

> [Stáhněte Composer](https://getcomposer.org/download/)

```
composer install
```


##### Node.js

> [Stáhněte Node.js](https://nodejs.org/en/)

```
# Windows:
npm install --no-bin-links

# Ostatní:
npm install
```

##### Nastavení prostředí

```
copy .env.example .env
php artisan key:generate
```

> *Nakonfigurujte soubor .env!*



##### Migrace databáze

```
php artisan migrate --seed
```


##### Zkompilování stylů a skriptů

```
npm run dev
```
##### Zkompilování stylů a skriptů do produkce

```
npm run prod
```
<?php

return [

    /*
    |--------------------------------------------------------------------------
    | Validation Language Lines
    |--------------------------------------------------------------------------
    |
    | The following language lines contain the default error messages used by
    | the validator class. Some of these rules have multiple versions such
    | as the size rules. Feel free to tweak each of these messages here.
    |
    */

    'accepted'             => 'Položka :attribute musí být příjmuta.',
    'active_url'           => 'Položka :attribute není validní URL adresa.',
    'after'                => 'Položka :attribute musí být datum po datu :date.',
    'after_or_equal'       => 'Položka :attribute musí být datum po nebo rovno datu :date.',
    'alpha'                => 'Položka :attribute může obsahovat pouze písmena.',
    'alpha_dash'           => 'Položka :attribute může obsahovat pouze písmena, čísla a pomlčky.',
    'alpha_num'            => 'Položka :attribute může obsahovat pouze písmena a čísla.',
    'array'                => 'Položka :attribute musí být seznam.',
    'before'               => 'Položka :attribute musí být datum před datem :date.',
    'before_or_equal'      => 'Položka :attribute musí být datum před nebo rovno datu :date.',
    'between'              => [
        'numeric' => 'Položka :attribute musí být mezi hodnotami :min a :max.',
        'file'    => 'Položka :attribute musí být mezi velikostí :min a :max kilobytů.',
        'string'  => 'Položka :attribute musí být dlouhá :min až :max znaků.',
        'array'   => 'Položka :attribute musí obsahovat :min až :max položek.',
    ],
    'boolean'              => 'Položka :attribute musí být 1 nebo 0.',
    'confirmed'            => 'Potvrzení se neshoduje se zadanou hodnotou.',
    'date'                 => 'Položka :attribute není platné datum.',
    'date_format'          => 'Položka :attribute neodpovídá formátu :format.',
    'different'            => 'Položka :attribute a :other musí být rozdílné.',
    'digits'               => 'Položka :attribute musí obsahovat :digits číslic.',
    'digits_between'       => 'Položka :attribute musí obsahovat :min až :max číslic.',
    'dimensions'           => 'Položka :attribute má špatné rozměry.',
    'distinct'             => 'Položka :attribute má duplicitní hodnotu.',
    'email'                => 'Položka :attribute musí být validní e-mailová adresa.',
    'exists'               => 'Vybraná položka :attribute neexistuje.',
    'file'                 => 'Položka :attribute musí být soubor.',
    'filled'               => 'Položka :attribute musí být vyplněna.',
    'image'                => 'Položka :attribute musí být obrázek.',
    'in'                   => 'Vybraná položka :attribute je neplatná.',
    'in_array'             => 'Položka :attribute není v seznamu :other.',
    'integer'              => 'Položka :attribute musí být celé číslo.',
    'ip'                   => 'Položka :attribute musí být platná IP adresa.',
    'ipv4'                 => 'Položka :attribute musí být platná IPv4 adresa.',
    'ipv6'                 => 'Položka :attribute musí být platná IPv6 adresa.',
    'json'                 => 'Položka :attribute musí být platný JSON řetězec.',
    'max'                  => [
        'numeric' => 'Položka :attribute nesmí být větší než :max.',
        'file'    => 'Položka :attribute nesmí být větší než :max kilobytů.',
        'string'  => 'Položka :attribute nesmí být delší než :max znaků.',
        'array'   => 'Položka :attribute nesmí obsahovat více než :max položek.',
    ],
    'mimes'                => 'Položka :attribute musí být soubor ve formátu: :values.',
    'mimetypes'            => 'Položka :attribute musí být soubor ve formátu: :values.',
    'min'                  => [
        'numeric' => 'Hodnota položky :attribute musí být nejméně :min.',
        'file'    => 'Velikost položky :attribute musí být nejméně :min kilobytů.',
        'string'  => 'Délka položky :attribute musí být nejméně :min znaků.',
        'array'   => 'Položka :attribute musí obsahovat nejméně :min položek.',
    ],
    'not_in'               => 'Vybraná položka :attribute je neplatná.',
    'numeric'              => 'Položka :attribute musí být číslo.',
    'present'              => 'Položka :attribute musí být uvedena.',
    'regex'                => 'Formát položky :attribute je neplatný.',
    'required'             => 'Položka :attribute je povinná.',
    'required_if'          => 'Položka :attribute je povinná pokud :other je :value.',
    'required_unless'      => 'Položka :attribute je povinná dokud :other je mezi :values.',
    'required_with'        => 'Položka :attribute je povinná pokud :values je uvedeno.',
    'required_with_all'    => 'Položka :attribute je povinná pokud :values je uvedeno.',
    'required_without'     => 'Položka :attribute je povinná pokud :values není uvedeno.',
    'required_without_all' => 'Položka :attribute je povínná dokud žádné z :values není uvedeno.',
    'same'                 => 'Položka :attribute a :other musí být shodné.',
    'size'                 => [
        'numeric' => 'Velikost položky :attribute musí být :size.',
        'file'    => 'Velikost položky :attribute must být :size kilobytů.',
        'string'  => 'Délka položky :attribute musí být :size znaků.',
        'array'   => 'Položka :attribute musí obsahovat :size položek.',
    ],
    'string'               => 'Položka :attribute musí být řetězec.',
    'timezone'             => 'Položka :attribute musí být validní časová zóna.',
    'unique'               => 'Položka :attribute již existuje.',
    'uploaded'             => 'Nepodařilo se nahrát položku :attribute.',
    'url'                  => 'Položka :attribute není validní URL adresa.',

    /*
    |--------------------------------------------------------------------------
    | Custom Validation Language Lines
    |--------------------------------------------------------------------------
    |
    | Here you may specify custom validation messages for attributes using the
    | convention "attribute.rule" to name the lines. This makes it quick to
    | specify a specific custom language line for a given attribute rule.
    |
    */

    'custom' => [
        'attribute-name' => [
            'rule-name' => 'custom-message',
        ],
        'g-recaptcha-response' => [
            'required' => 'Ověřte prosím, že nejste robot.',
            'captcha' => 'Došlo k chybě při ověřování captcha.',
        ],
    ],

    /*
    |--------------------------------------------------------------------------
    | Custom Validation Attributes
    |--------------------------------------------------------------------------
    |
    | The following language lines are used to swap attribute place-holders
    | with something more reader friendly such as E-Mail Address instead
    | of "email". This simply helps us make messages a little cleaner.
    |
    */

    'attributes' => [
        'title' => 'název',
        'sort' => 'pořadí',
        'category_id' => 'kategorie',
        'file_info' => 'videozáznam'
    ],

];

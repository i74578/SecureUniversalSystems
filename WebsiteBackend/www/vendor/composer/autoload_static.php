<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInit1e11bc7d116985516d6910532ee95400
{
    public static $prefixLengthsPsr4 = array (
        'P' => 
        array (
            'Psr\\Log\\' => 8,
            'PhpMqtt\\Client\\' => 15,
        ),
        'M' => 
        array (
            'MyCLabs\\Enum\\' => 13,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'Psr\\Log\\' => 
        array (
            0 => __DIR__ . '/..' . '/psr/log/src',
        ),
        'PhpMqtt\\Client\\' => 
        array (
            0 => __DIR__ . '/..' . '/php-mqtt/client/src',
        ),
        'MyCLabs\\Enum\\' => 
        array (
            0 => __DIR__ . '/..' . '/myclabs/php-enum/src',
        ),
    );

    public static $classMap = array (
        'Composer\\InstalledVersions' => __DIR__ . '/..' . '/composer/InstalledVersions.php',
        'Stringable' => __DIR__ . '/..' . '/myclabs/php-enum/stubs/Stringable.php',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInit1e11bc7d116985516d6910532ee95400::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInit1e11bc7d116985516d6910532ee95400::$prefixDirsPsr4;
            $loader->classMap = ComposerStaticInit1e11bc7d116985516d6910532ee95400::$classMap;

        }, null, ClassLoader::class);
    }
}

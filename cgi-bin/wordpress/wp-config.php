<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the installation.
 * You don't have to use the web site, you can copy this file to "wp-config.php"
 * and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * Database settings
 * * Secret keys
 * * Database table prefix
 * * Localized language
 * * ABSPATH
 *
 * @link https://wordpress.org/support/article/editing-wp-config-php/
 *
 * @package WordPress
 */

// ** Database settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define( 'DB_NAME', 'wordpress' );

/** Database username */
define( 'DB_USER', 'root' );

/** Database password */
define( 'DB_PASSWORD', 'jongho' );

/** Database hostname */
define( 'DB_HOST', 'mariadb' );

/** Database charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8' );

/** The database collate type. Don't change this if in doubt. */
define( 'DB_COLLATE', '' );

/**#@+
 * Authentication unique keys and salts.
 *
 * Change these to different unique phrases! You can generate these using
 * the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}.
 *
 * You can change these at any point in time to invalidate all existing cookies.
 * This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define( 'AUTH_KEY',          'LlCsx3XUeJ.ueSlGxuxd1n{ALJeE{,]F:3]ITjlz##PX[YB7e,YTme`fAZyMD6`M' );
define( 'SECURE_AUTH_KEY',   'ZY89TIhMSq<{`~lp54TrENv%H1Y_YY[wupco8a.3($Sj_cCId_h ^o*hH1}/9uk^' );
define( 'LOGGED_IN_KEY',     '}0xd_N*/_7CU30{)f./ %Oc--=7MdkBgiZ8<Vj.$/KW,00`n wyAxrT/b;05lC,-' );
define( 'NONCE_KEY',         'giTo7~jU>&b5TRFV).^U&8BWzW U-^1`8vHwf&Dd:mjor%*SEVk;x?:}yIj&UPKR' );
define( 'AUTH_SALT',         'I$OAuKcmU+svJ]>{M_#1+:vD-:?8=gPrp_HVskv*D-46V.tZn+X[uXQa9neW6?a_' );
define( 'SECURE_AUTH_SALT',  '4DeOHy*8=,>!5lG72@>J~oe0Sm HH%Xq_!TmyqZdT4>z_NL(4-n0e){y0n/M{]Vv' );
define( 'LOGGED_IN_SALT',    '_17:&N;Z$>g/2-54Hg!*=pvwBIC#]ZdzGut?5GyYGAEpff[y%OQm4gsZ^45p|0@H' );
define( 'NONCE_SALT',        '[V/JKt`Z0ObiM{OpIO`8GG+)$cBa7nu$m8a:1vo4=zC|N]fcN]ZRudx}oMMLwW8q' );
define( 'WP_CACHE_KEY_SALT', 'e%#BJ^?dS<v]vMl6<rVG4aE_4Ix[Q~J3D<<:L/aeMIV$1A#s3(_rSgjg6JiQ]Ob;' );


/**#@-*/

/**
 * WordPress database table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 */
$table_prefix = 'wp_';


/* Add any custom values between this line and the "stop editing" line. */



/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 *
 * For information on other constants that can be used for debugging,
 * visit the documentation.
 *
 * @link https://wordpress.org/support/article/debugging-in-wordpress/
 */
if ( ! defined( 'WP_DEBUG' ) ) {
	define( 'WP_DEBUG', false );
}

/* That's all, stop editing! Happy publishing. */

/** Absolute path to the WordPress directory. */
if ( ! defined( 'ABSPATH' ) ) {
	define( 'ABSPATH', __DIR__ . '/' );
}

/** Sets up WordPress vars and included files. */
require_once ABSPATH . 'wp-settings.php';

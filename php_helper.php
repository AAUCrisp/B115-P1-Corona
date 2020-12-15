<?php
/**
 * Created by PhpStorm.
 * User: lord_
 * Date: 18-05-2018
 * Time: 10:18
 */

class php_helper {

	/**
	 * # Move Array Item (Key/Value) to bottom of Array #
	 *
	 * @param array $array the array to edit
	 * @param string $key name of the key to move
	 * @param bool $value_instead enable if you wanna find and move a value instead of key
	 *
	 * @return array
	 */
	static function array_to_bottom($array, $key, $value_instead = false) {

		if ($value_instead) {
			if (($array_key = array_search($key, $array)) !== false) {
				unset($array[$array_key]);
			}
			array_push($array,$key);
		}
		else {
			$value = $array[$key];
			unset($array[$key]);
			array_push($array, $value);
		}
		return $array;
	}

	/**
	 * # Move Array Item (Key/Value) to top of Array #
	 *
	 * @param array $array the array to edit
	 * @param string $key name of the key to move
	 * @param bool $value_instead enable if you wanna find and move a value instead of key
	 *
	 * @return array
	 */
	static function array_to_top($array, $key, $value_instead = false) {

		if ($value_instead) {
			if (($array_key = array_search($key, $array)) !== false) {
				unset($array[$array_key]);
			}
			array_unshift($array , $key);
		}
		else {
			$value = array($key => $array[$key]);
			unset($array[$key]);
			$array = $value + $array;
		}

		return $array;
	}


	/**
	 * # Find Value in multidimensional array #
	 * Will keep digging until the values isn't arrays anymore
	 *
	 * @param mixed $look_for value to look for
	 * @param array $array the array to look through
	 * @param string $at_key the name of the key to look in (optional)
	 * @param array $history _don't set_... used for deep arrays
	 *
	 * @return int|string|array with the path to the found value
	 */
	static function in_multi_array($look_for, $array, $at_key = false, $history = false) {

		if (is_array($array)) {
			// Loop over array and check for value
			foreach ($array AS $key => $item) {
				// Set first layer of history
				$history[] = $key;
				// Make sure it's actually an array
				if (is_array($item)) {
					// If only looking in specific keys
					if ( $at_key && isset($item [$at_key]) && $item [$at_key] === $look_for ) {
						$result = $history;
					} // Else check the whole array for the value
					elseif ( in_array($look_for, $item, 1) && $at_key === false ) {
						$found_key = array_search( $look_for, $item, 1 );
						$history[] = $found_key;
						$result = $history;
					}// If still no result, look deeper
					elseif (!isset($result)) {
						// Let the function call itself, will return the result if possible
						$result = self::in_multi_array($look_for, $item, $at_key, $history);
					}
				}
				if (isset($result)) break;
				array_pop( $history );
			}
			if (isset($result)) {
				return $result;
			}
		}

	}

	/**
	 * # Hash String #
	 *
	 * Hashes the string sent to it, and returns an array with
	 * both the hash & salt string in it.
	 *
	 * 	 * ## Config options are ##
	 * - **use_salt** *(bool)* : Whether to use a salt string or not (`default = true`)
	 * - **encryption** *(const)* : The encryption used (`default = PASSWORD_BCRYPT`)
	 *
	 * @param $input
	 * @param array $config
	 *
	 * @return array with 'hash' & 'salt' keys
	 * @throws Exception
	 */
	static function hash_string($input, $config = []) {
		$defaults = [
			'use_salt' => true,
			'encryption' => PASSWORD_BCRYPT
		];
		$config = array_merge($defaults, $config);

		//Create random bytes
		$salt_byte = random_bytes(15);
		//Make the bytes into a readable string (to save to the database)
		$salt_string = $config['use_salt'] ? bin2hex($salt_byte) : "";
		//Put the salt-string after the password for the hashing for both creation and login
		$string_hashed = password_hash($input . $salt_string, $config['encryption']);

		return ['hash' => $string_hashed, 'salt' => $salt_string];
	}

	/**
	 * # Truncate String #
	 *
	 * @param string $string
	 * @param int $length
	 *
	 * @return bool|string
	 */
	static function truncate_string($string, $length = 100) {
//		$rest = substr("abcdef", -3, -1); // returns "de"
		$result = substr($string, 0, $length);
		$result = "<span class='truncated'>$result...</span>";
		return $result;
	}

}
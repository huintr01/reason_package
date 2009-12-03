<?php

/**
 * Functions for manipulating and locating images.
 *
 * The function library "images.php" contains functions that help with
 * displaying images.
 *
 * @author Eric Naeseth <enaeseth+reason@gmail.com>
 * @package reason
 * @subpackage function_libraries
 */

/**
 * Gets the filename of an image relative to the photostock directory.
 * @param mixed $image the ID of an image entity or an image entity object
 * @param string $size the desired scaling of the image: thumbnail/thumb/tn,
 *                     standard/default, or original/full (which is not always
 *                     available)
 * @return string the image's filename, or NULL if an invalid value was given
 *                for $image
 */
function reason_get_image_filename($image, $size='standard')
{
	if (is_object($image)) {
		if (!is_callable(array(&$image, 'get_value'))) {
			trigger_error('given object does not look like an entity to me: '.
				var_export($image, true), WARNING);
			return null;
		}
	} else if (is_numeric($image)) {
		$id = (int) $image;
		$image = new Entity($id);
		if (!$image->get_values()) {
			trigger_error('no entity exists with ID '.$id, WARNING);
			return null;
		}
	} else {
		trigger_error('must pass an entity or an image entity ID; instead '.
			'got '.var_export($image, true), WARNING);
		return null;
	}
	
	if ($image->get_value('type') != id_of('image')) {
		trigger_error('given entity is not an image; is actually a '.
			var_export($image->get_value('type'), true), WARNING);
		return null;
	}
	
	return reason_format_image_filename($image->id(),
		$image->get_value('image_type'), $size);
}

/**
 * Creates an image filename (relative to the photostock directory).
 * 
 * This function is mainly useful for code that is handling image uploads; for
 * a safer version, see {@link reason_get_image_filename()}.
 *
 * No checks will be performed to see if an entity with the given ID exists,
 * if the entity refers to an image, or if the correct file extension was
 * given.
 *
 * @param int $id an image entity ID
 * @param string $extension the image's type (file extension)
 * @param string $size the size of image
 */
function reason_format_image_filename($id, $extension, $size='standard')
{
	$size_suffix = _normalize_image_size_name($size);
	if ($size_suffix === null) {
		trigger_error('unrecognized image size '.var_export($size, true),
			WARNING);
		return null;
	}
	
	$filename_impl = (defined('REASON_IMAGE_FILE_NAMER_IMPLEMENTATION'))
		? REASON_IMAGE_FILE_NAMER_IMPLEMENTATION
		: 'image_flat_id_filename';
	
	if (!is_callable($filename_impl)) {
		trigger_error('file namer implementation function '.
			var_export($filename_impl, true).' does not exist', FATAL);
	}
	
	return call_user_func($filename_impl, $id, $size_suffix, $extension);
}

/**
 * Gets the URL of an image relative to the server root.
 * @param mixed $image the ID of an image entity or an image entity object
 * @param string $size the desired scaling of the image: thumbnail/thumb/tn,
 *                     standard/default, or original/full (which is not always
 *                     available)
 * @return string the image's relative URL, or NULL if an invalid value was
 *                given for $image
 */
function reason_get_image_url($image, $size='standard')
{
	$filename = reason_get_image_filename($image, $size);
	return ($filename) ? WEB_PHOTOSTOCK.$filename : null;
}

/**
 * Gets the full server path of an image.
 * @param mixed $image the ID of an image entity or an image entity object
 * @param string $size the desired scaling of the image: thumbnail/thumb/tn,
 *                     standard/default, or original/full (which is not always
 *                     available)
 * @return string the image file's path, or NULL if an invalid value was given
 *                for $image
 */
function reason_get_image_path($image, $size='standard')
{
	$filename = reason_get_image_filename($image, $size);
	return ($filename) ? PHOTOSTOCK.$filename : null;
}

/**
 * @access private
 */
function _normalize_image_size_name($size_name)
{
	static $name_map = array(
		'thumbnail' => 'tn',
		'thumb' => 'tn',
		'tn' => 'tn',
		'standard' => '',
		'default' => '',
		'original' => 'orig',
		'orig' => 'orig',
		'full' => 'orig'
	);
	
	return (isset($name_map[$size_name]))
		? $name_map[$size_name]
		: null;
}

/**
 * Image filename implementation function: names images after their ID in
 * a flat structure.
 * @param int $id an image ID
 * @param string $size  image size suffix
 * @param string $type value of the image's `image_type` property
 * @return string image filename, relative to the photostock folder
 */
function image_flat_id_filename($id, $size, $type)
{
	if (!empty($size))
		$size = "_$size";
	
	return "{$id}{$size}.{$type}";
}

/**
 * Image filename implementation function: names images after their ID in
 * a two-tiered structure; for example, an image with an entity ID of 12345
 * would be stored at (normal size):
 *     PHOTOSTOCK/1/12/12345.jpg
 *
 * @param int $id an image ID
 * @param string $size  image size suffix
 * @param string $type value of the image's `image_type` property
 * @return string image filename, relative to the photostock folder
 */
function image_tiered_id_filename($id, $size, $type)
{
	if (!empty($size))
		$size = "_$size";
	
	$expanded_id = sprintf("%03d", $id);
	$parts = array(
		substr($expanded_id, 0, 1),
		substr($expanded_id, 0, 2),
		"{$expanded_id}{$size}.{$type}"
	);
	return implode("/", $parts);
}
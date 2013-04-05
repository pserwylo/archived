package com.serwylo.brewdiary.data
{

public class NoteType
{

	[Embed(source="../icons/exclamation.png")]
	public static const IMG_ERROR:Class;

	[Embed(source="../icons/note_edit.png")]
	public static const IMG_NOTE:Class;

	[Embed(source="../icons/lightbulb.png")]
	public static const IMG_IDEA:Class;

	
	public static const NOTE:Number = 1;
	public static const ERROR:Number = 2;
	public static const IDEA:Number = 3;

	public var noteTypeLabel:String;
	public var noteTypeSerial:Number;

}

}

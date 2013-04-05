/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 29/12/10
 * Time: 7:20 PM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.core
{
public class Controller
{

	public function get view():View
	{
		throw new Error( "Controller.view is abstract" );
	}

	public function Controller()
	{
	}
}
}

/**
 * Created by IntelliJ IDEA.
 * User: pete
 * Date: 29/12/10
 * Time: 7:21 PM
 * To change this template use File | Settings | File Templates.
 */
package com.serwylo.brewdiary.core
{

import spark.components.SkinnableContainer;
import spark.layouts.VerticalLayout;

public class View extends SkinnableContainer
{
	public function View()
	{
		this.layout = new VerticalLayout();
	}
}
}

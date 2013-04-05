package com.serwylo.brewdiary.views.overview
{
import com.serwylo.brewdiary.views.brew.BrewController;

import mx.collections.ArrayCollection;
import mx.collections.ArrayList;

[Bindable]
public class OverviewModel
{

	public var batchController:BrewController;

	public var brewList:ArrayCollection;
	public var statusList:ArrayCollection;

}

}

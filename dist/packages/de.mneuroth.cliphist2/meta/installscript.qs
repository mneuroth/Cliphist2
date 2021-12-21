function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/cliphist2.exe", "@StartMenuDir@/Cliphist2.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/cliphist2_256x256.ico",
            "description=Cliphist 2");
        component.addOperation("CreateShortcut", "@TargetDir@/cliphist2.exe", "@DesktopDir@/Cliphist2.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/cliphist2_256x256.ico");
    }
}

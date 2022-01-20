<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ Page Language="VB" %>
<html dir="ltr" xmlns="http://www.w3.org/1999/xhtml">

<head runat="server">
<meta content="en-us" http-equiv="Content-Language" />
<meta content="text/html; charset=utf-8" http-equiv="Content-Type" />
<title>Untitled 1</title>
<style type="text/css">
@import url('style.css');
.auto-style1 {
	vertical-align: text-top;
}
.auto-style2 {
	text-align: center;
}
.auto-style3 {
	font-family: "Trebuchet MS", "Lucida Sans Unicode", "Lucida Grande", "Lucida Sans", Arial, sans-serif;
}
</style>
</head>

<body>

<form id="form1" runat="server">
	<img class="auto-style1" height="288" src="Picture12.png" style="float: left" width="266" /><div class="auto-style2">
		<asp:CreateUserWizard id="CreateUserWizard1" runat="server" Font-Names="Trebuchet MS, Lucida Sans Unicode, Lucida Grande, Lucida Sans, Arial, sans-serif">
			<WizardSteps>
				<asp:CreateUserWizardStep runat="server" />
				<asp:CompleteWizardStep runat="server" />
			</WizardSteps>
		</asp:CreateUserWizard>
	</div>
	<span class="auto-style3">&nbsp;</span></form>

</body>

</html>

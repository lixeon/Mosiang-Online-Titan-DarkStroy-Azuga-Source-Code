<%@ Control Language="c#" AutoEventWireup="false" Codebehind="forgotpassword.ascx.cs" Inherits="web.control.forgotpassword" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="60%" border="0">
	<TR>
		<TD colspan="3"><b>找回登陸密碼</b></TD>
	</TR>
	<tr>
		<td>登陸帳號</td>
		<td>
			<asp:TextBox id="tbuserid" runat="server"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ErrorMessage="*" ControlToValidate="tbuserid"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td>二級密碼</td>
		<td>
			<asp:TextBox id="tbkey" runat="server" TextMode="Password"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="tbkey"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td>電子郵箱</td>
		<td>
			<asp:TextBox id="tbemail" runat="server"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="RequiredFieldValidator2" runat="server" ErrorMessage="*" ControlToValidate="tbemail"></asp:RequiredFieldValidator></td>
	</tr>
	<TR>
		<TD style="WIDTH: 84px">驗證碼</TD>
		<TD style="WIDTH: 151px">
			<asp:TextBox id="tbverifycode" runat="server" MaxLength="4" Columns="4"></asp:TextBox><IMG id="imgVerify" style="WIDTH: 56px; CURSOR: hand; HEIGHT: 20px" onclick="this.src=this.src"
				height="20" alt="看不清？點擊更換" src="./VerifyCode.aspx" width="56"></TD>
		<TD>
			<asp:RequiredFieldValidator id="Requiredfieldvalidator5" runat="server" ErrorMessage="*" ControlToValidate="tbverifycode"></asp:RequiredFieldValidator>請輸入圖片中的四位數字</TD>
	</TR>
	<TR>
		<TD style="HEIGHT: 21px" align="center" colSpan="3"></TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" style="HEIGHT: 21px">提示：密碼找回後將被修改為123456,請速登陸網站後修改</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3"><FONT face="細明體"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnreset" runat="server" Text="找回密碼" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="帳號格式錯誤!" ControlToValidate="tbuserid"
	ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
<asp:RegularExpressionValidator id="RegularExpressionValidator2" runat="server" ErrorMessage="二級密碼格式錯誤!" ControlToValidate="tbkey"
	ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
<asp:RegularExpressionValidator id="RegularExpressionValidator3" runat="server" ControlToValidate="tbemail" ErrorMessage="郵箱格式錯誤!"
	ValidationExpression="\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*"></asp:RegularExpressionValidator><BR>
<asp:RegularExpressionValidator id="RegularExpressionValidator6" runat="server" ControlToValidate="tbverifycode"
	ErrorMessage="驗證碼格式錯誤!" ValidationExpression="\d{4}$"></asp:RegularExpressionValidator><BR>

<%@ Control Language="c#" AutoEventWireup="false" Codebehind="modifypassword.ascx.cs" Inherits="web.control.modifypassword" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="60%" border="0">
	<TR>
		<TD colspan="3"><b>修改登陸密碼</b></TD>
	</TR>
	<tr>
		<td style="WIDTH: 84px">登陸帳號</td>
		<td style="WIDTH: 151px">
			<asp:TextBox id="tbuserid" runat="server"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ErrorMessage="*" ControlToValidate="tbuserid"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td style="WIDTH: 84px">舊密碼</td>
		<td style="WIDTH: 151px">
			<asp:TextBox id="tbolduserpwd" runat="server" TextMode="Password"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="tbolduserpwd"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td style="WIDTH: 84px">新密碼</td>
		<td style="WIDTH: 151px">
			<asp:TextBox id="tbnewuserpwd" runat="server" TextMode="Password"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="RequiredFieldValidator2" runat="server" ErrorMessage="*" ControlToValidate="tbnewuserpwd"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td style="WIDTH: 84px">確認新密碼</td>
		<td style="WIDTH: 151px">
			<asp:TextBox id="tbrenewuserpwd" runat="server" TextMode="Password"></asp:TextBox></td>
		<td>僅限4~12位a-z,A-Z字母及數字</td>
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
		<TD align="center" colSpan="3" height="10"><FONT face="細明體"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnmodify" runat="server" Text="修改密碼" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="帳號格式錯誤!" ControlToValidate="tbuserid"
	ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
<asp:RegularExpressionValidator id="RegularExpressionValidator2" runat="server" ErrorMessage="舊密碼格式錯誤!" ControlToValidate="tbolduserpwd"
	ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
<asp:RegularExpressionValidator id="RegularExpressionValidator4" runat="server" ControlToValidate="tbnewuserpwd"
	ErrorMessage="新密碼格式錯誤!" ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><BR>
<asp:CompareValidator id="CompareValidator1" runat="server" ErrorMessage="兩次密碼不一致!" ControlToValidate="tbrenewuserpwd"
	ControlToCompare="tbnewuserpwd"></asp:CompareValidator><BR>
<asp:RegularExpressionValidator id="RegularExpressionValidator6" runat="server" ControlToValidate="tbverifycode"
	ErrorMessage="驗證碼格式錯誤!" ValidationExpression="\d{4}$"></asp:RegularExpressionValidator>

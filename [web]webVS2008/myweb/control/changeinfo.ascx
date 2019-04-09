<%@ Control Language="c#" AutoEventWireup="false" Codebehind="changeinfo.ascx.cs" Inherits="web.control.changeinfo" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="60%" border="0">
	<TR>
		<TD colspan="3"><b>修改二級密碼與EMAIL</b></TD>
	</TR>
	<TR>
		<TD colSpan="3">
			<P><FONT face="宋体">本功能保護玩家的賬戶,修改二級密碼與EMAIL需要<%=Application["game.changeinfogold"]%>金幣</FONT></P>
		</TD>
	</TR>
	<tr>
		<td>登陸帳號</td>
		<td>
			<asp:TextBox id="tbuserid" runat="server" ReadOnly="True" ForeColor="White" BackColor="#E0E0E0"></asp:TextBox></td>
		<td><FONT face="宋体"></FONT></td>
	</tr>
	<tr>
		<td>新二級密碼</td>
		<td>
			<asp:TextBox id="tbkey" runat="server" TextMode="Password" Height="24px"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="tbkey"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td>確認新二級密碼</td>
		<td>
			<asp:TextBox id="tbrekey" runat="server" TextMode="Password" Height="24px"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="Requiredfieldvalidator1" runat="server" ErrorMessage="*" ControlToValidate="tbkey"></asp:RequiredFieldValidator>僅限4~12位a-z,A-Z字母及數字</td>
	</tr>
	<tr>
		<td>新電子郵箱EMAIL</td>
		<td>
			<asp:TextBox id="tbemail" runat="server"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="RequiredFieldValidator2" runat="server" ErrorMessage="*" ControlToValidate="tbemail"></asp:RequiredFieldValidator></td>
	</tr>
	<tr>
		<td>確認新電子郵箱EMAIL</td>
		<td>
			<asp:TextBox id="tbreemail" runat="server"></asp:TextBox></td>
		<td>
			<asp:RequiredFieldValidator id="Requiredfieldvalidator3" runat="server" ErrorMessage="*" ControlToValidate="tbemail"></asp:RequiredFieldValidator></td>
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
		<TD align="center" colSpan="3"><FONT face="細明體"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="確認修改二級密碼與EMAIL" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<br>
<asp:RegularExpressionValidator id="RegularExpressionValidator2" runat="server" ErrorMessage="二級密碼格式錯誤!" ControlToValidate="tbkey"
	ValidationExpression="^[A-Za-z0-9]{4,12}$"></asp:RegularExpressionValidator><br>
<asp:RegularExpressionValidator id="RegularExpressionValidator3" runat="server" ControlToValidate="tbemail" ErrorMessage="郵箱格式錯誤!"
	ValidationExpression="\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*"></asp:RegularExpressionValidator><BR>
<asp:RegularExpressionValidator id="RegularExpressionValidator6" runat="server" ControlToValidate="tbverifycode"
	ErrorMessage="驗證碼格式錯誤!" ValidationExpression="\d{4}$"></asp:RegularExpressionValidator><BR>
<asp:CompareValidator id="CompareValidator1" runat="server" ControlToValidate="tbreemail" ErrorMessage="兩次EMAIL輸入不一致"
	ControlToCompare="tbemail"></asp:CompareValidator><br>
<asp:CompareValidator id="CompareValidator2" runat="server" ControlToValidate="tbrekey" ErrorMessage="兩次二級密碼輸入不一致"
	ControlToCompare="tbkey"></asp:CompareValidator>

<%@ Control Language="c#" AutoEventWireup="false" Codebehind="bank.ascx.cs" Inherits="web.control.bank" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3">
			<P><b>網上銀行:</b></P>
			<P>由於倉庫的可放金錢容量太小，在這裡可以將您角色上的金錢全部存入銀行哦，銀行為帳號形式，同一帳號所有角色均可以在此存錢取錢，存錢上限為5000億，取錢上限為加上角色自帶的共20億</P>
			<P><FONT face="宋体"></FONT>&nbsp;</P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 88px"><strong><span style="FONT-WEIGHT:bold;COLOR:#5d51f1"><%=Session["userid"]%></span></strong></td>
		<td></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px">銀行存款</td>
		<td style="WIDTH: 88px"><strong><span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=webbank%></span>/<strong><span style="FONT-WEIGHT:bold;COLOR:#5d51f1">500000000000</span></strong></strong></td>
		<td><FONT face="宋体">如顯示錯誤請重登陸網站</FONT></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 20px">角色列表</td>
		<td style="WIDTH: 88px; HEIGHT: 20px">
			<asp:DropDownList id="ddchalist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇需要操作的角色</td>
	</tr>
	<TR>
		<TD style="WIDTH: 103px; HEIGHT: 20px">輸入金額</TD>
		<TD style="WIDTH: 88px; HEIGHT: 20px">
			<asp:TextBox id="tbmoney" runat="server" MaxLength="14" Width="169px"></asp:TextBox></TD>
		<TD style="HEIGHT: 20px">
			<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ControlToValidate="tbmoney" ErrorMessage="*"></asp:RequiredFieldValidator>請輸入存/取金額</TD>
	</TR>
	<TR>
		<TD align="center" colSpan="3" height="10">
			<asp:RadioButton id="rbput" runat="server" Text="存錢到銀行" GroupName="bank" Checked="True"></asp:RadioButton>
			<asp:RadioButton id="rbget" runat="server" Text="取錢到角色" GroupName="bank"></asp:RadioButton></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="銀行操作" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>
<P>
	<asp:RegularExpressionValidator id="RegularExpressionValidator1" runat="server" ErrorMessage="金錢格式不對" ValidationExpression="^\d{1,10}$"
		ControlToValidate="tbmoney"></asp:RegularExpressionValidator><br>
<P></P>

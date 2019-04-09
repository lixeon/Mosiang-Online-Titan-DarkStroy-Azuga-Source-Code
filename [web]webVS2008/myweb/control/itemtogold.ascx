<%@ Control Language="c#" AutoEventWireup="false" Codebehind="itemtogold.ascx.cs" Inherits="web.control.itemtogold" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3" style="HEIGHT: 290px">
			<P><b>出售中介物品為金幣功能:</b></P>
			<P><FONT color="#000000"><FONT color="#0000ff"><STRONG>中介物品名稱及價格:</STRONG></FONT></FONT></P>
			<P>
			營商許可證(三天)&nbsp; 回收價格9</P>
		<P>營商許可證(七天)&nbsp; 回收價格90</P>
		<P>營商許可證(十五天)&nbsp; 回收價格 450</P>
		<P>營商許可證不可解封使用,否則將不能回收</P>
		<P>注意: 需要將中介物品變換成金幣的玩家,請將許可證放入角色百寶物品欄,需要變換多少放多少,系統將一次性變換所有在此角色上的所有許可證為金幣</P>
		<P>如果你不想變換所有的許可證,可以將許可證放入百寶倉庫</P>
		</TD></TR>

	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 120px"><strong><%=Session["userid"]%></strong></td>
		<td></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 12px">角色列表</td>
		<td style="WIDTH: 120px; HEIGHT: 12px">
			<asp:DropDownList id="ddchalist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 12px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇角色</td>
	</tr>
	<TR>
		<TD align="center" colSpan="3" height="10"><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="確認變換" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>

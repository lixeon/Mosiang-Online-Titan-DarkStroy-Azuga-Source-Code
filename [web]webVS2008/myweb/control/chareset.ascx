<%@ Control Language="c#" AutoEventWireup="false" Codebehind="chareset.ascx.cs" Inherits="web.control.chareset" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table cellspacing="1" cellpadding="1" width="528" border="0">
	<TR>
		<TD colspan="3" style="HEIGHT: 290px">
			<P><b>在線轉生:</b></P>
			<P>轉生需要退出遊戲，每個角色最多可以轉生<span style="FONT-WEIGHT:bold;COLOR:#5d51f1"><%=Application["game.charesetcount"]%></span>次</P>
			<p>轉生規則如下：</p>
			<P>1.第一次轉生需要等級為<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.charesetflv"]%></span>級，以後每轉一次增加<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.charesetlvstep"]%></span>級</P>
			<P>2.第一次轉生需要遊戲幣<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.charesetfmoney"]%></span>元，以後每轉一次增加<span style="FONT-WEIGHT:bold;COLOR:#ff3333"><%=Application["game.charesetmoneystep"]%></span>元</P>
			<P>3.角色轉生後，角色等級為<FONT color="#0000ff">1</FONT>級、經驗為<FONT color="#0000ff">1</FONT>、力量為<FONT color="#0000ff">12</FONT>、敏捷為<FONT color="#0000ff">12</FONT>、體質為<FONT color="#0000ff">12</FONT>、心脈為<FONT color="#0000ff">12、</FONT></SPAN> 
				可自由分配點數<%=Application["game.charesetgivepoint"]%>*轉數</P>
			<P>4.<span style="FONT-WEIGHT:bold;COLOR:#5d51f1">15</span>轉以後每轉所得100屬性點，系統自動加至體質</P>
			<P>5.<span style="FONT-WEIGHT:bold;COLOR:#5d51f1">30</span>轉以後開放超級轉世,每轉都極為堅苦,望玩家量力而行.每轉世所得50屬性點,系統自動加至體質</P>
			<P><FONT face="宋体">注意:30轉世開始每轉需要<span style="FONT-WEIGHT:bold;COLOR:#ff3333">50億(5,000,000,000)</span>遊戲幣或<SPAN style="FONT-WEIGHT: bold; COLOR: #ff3333">500</SPAN>金幣,以後每增加一轉將增加<span style="FONT-WEIGHT:bold;COLOR:#ff3333">10億(1,000,000,000)遊戲幣或500金幣</span>,遊戲幣請放入網上銀行,系統將自動去網上銀行扣款.</FONT></P>
		</TD>
	</TR>
	<tr>
		<td style="WIDTH: 103px">登陸帳號</td>
		<td style="WIDTH: 120px"><strong><%=Session["userid"]%></strong></td>
		<td></td>
	</tr>
	<tr>
		<td style="WIDTH: 103px; HEIGHT: 14px">角色列表</td>
		<td style="WIDTH: 120px; HEIGHT: 14px">
			<asp:DropDownList id="ddchalist" runat="server"></asp:DropDownList></td>
		<td style="HEIGHT: 14px">
			<asp:RequiredFieldValidator id="Requiredfieldvalidator6" runat="server" ErrorMessage="*" ControlToValidate="ddchalist"></asp:RequiredFieldValidator>請選擇需要轉生的角色</td>
	</tr>
	<TR>
		<TD align="center" colSpan="3" height="10">
			<asp:RadioButton id="rbmoney" runat="server" Text="遊戲幣支付" GroupName="paymode" Checked="True"></asp:RadioButton>
			<asp:RadioButton id="rbgold" runat="server" Text="金幣支付(30轉以後可以使用)" GroupName="paymode"></asp:RadioButton></TD>
	</TR>
	<TR>
		<TD colspan="3" align="center">
			<asp:Button id="btnedit" runat="server" Text="確認轉生" CssClass="button_1"></asp:Button>
		</TD>
	</TR>
</table>

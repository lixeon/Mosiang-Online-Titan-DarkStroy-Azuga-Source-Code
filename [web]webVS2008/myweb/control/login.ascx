<%@ Control Language="c#" AutoEventWireup="false" Codebehind="login.ascx.cs" Inherits="web.control.login" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>

<%
if(Session["userid"]==null)
{
%>
<DIV id="login" align="center"><IMG alt="賬號登錄" src="images/col_login.gif">
	<DL>
		<DT>賬&nbsp;號
			<DD>
				<asp:textbox id="tbuserid" runat="server" Columns="16" MaxLength="16"></asp:textbox><FONT face="細明體">&nbsp;</FONT>
				<DT>密 碼
					<DD>
						<asp:textbox id="tbuserpwd" runat="server" Columns="16" MaxLength="16" TextMode="Password"></asp:textbox><FONT face="細明體">&nbsp;</FONT>
						<DT>驗證碼
							<DD>
								<asp:textbox id="tbverifycode" runat="server" Columns="4" MaxLength="4" Width="40px"></asp:textbox><IMG id="imgVerify" style="WIDTH: 56px; CURSOR: hand; HEIGHT: 20px" onclick="this.src=this.src"
									alt="看不清？點擊更換" src="VerifyCode.aspx"></DD></DL>
	<DIV class="btn">
		<asp:button id="btnlogin" style="CURSOR: hand" runat="server" CssClass="submit"></asp:button>&nbsp;<A href="register.aspx"><IMG src="images/btn_reg.gif" border="0"></A><A href="forgetpassword.aspx"><IMG src="images/btn_forget.gif" border="0"></A>
	</DIV>
</DIV>
<%}
else
{%>
<DIV align="center" id="login">
	<img alt="賬號登錄" src="images/col_login.gif">
	<DL>
		<b>
			[墨香]
			歡迎您</b>
		<br>
		<DT>帳 號
			<DD>
				<%=Session["userid"]%>
				<DT>等 級
					<DD>
						<%=weblevel%>
						<DT>金 幣<DD><%=Session["webgold"]%>個</DD>
	</DL>
	<DIV class="btn">
		<asp:Button CssClass="button_1" id="Button1" Text="退出網站" runat="server"></asp:Button>
	</DIV>
</DIV>
<%}%>

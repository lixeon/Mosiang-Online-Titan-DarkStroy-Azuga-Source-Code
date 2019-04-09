<%@ Control Language="c#" AutoEventWireup="false" Codebehind="guild.ascx.cs" Inherits="web.control.guild" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<FONT face="宋体">
	<asp:datagrid id="DataGrid1" runat="server" AutoGenerateColumns="False" Width="100%" BorderColor="#CC9966"
		BorderStyle="None" BorderWidth="1px" BackColor="White" CellPadding="4" Height="160px">
		<SelectedItemStyle Font-Bold="True" ForeColor="#663399" BackColor="#FFCC66"></SelectedItemStyle>
		<ItemStyle ForeColor="#330099" BackColor="White"></ItemStyle>
		<HeaderStyle Font-Bold="True" ForeColor="#FFFFCC" BackColor="#990000"></HeaderStyle>
		<FooterStyle ForeColor="#330099" BackColor="#FFFFCC"></FooterStyle>
		<Columns>
			<asp:TemplateColumn>
				<HeaderTemplate>
					<center>排名</center>
				</HeaderTemplate>
				<ItemTemplate>
					<center><%=i++%></center>
				</ItemTemplate>
			</asp:TemplateColumn>
			<asp:TemplateColumn>
				<HeaderTemplate>
					<center>幫會名</center>
				</HeaderTemplate>
				<ItemTemplate>
					<center><%# sys.ConvertToBig5((DataBinder.Eval(Container.DataItem,"GuildName").ToString()),950)%></center>
				</ItemTemplate>
			</asp:TemplateColumn>
			<asp:TemplateColumn>
				<HeaderTemplate>
					<center>幫主名</center>
				</HeaderTemplate>
				<ItemTemplate>
					<center><%# sys.ConvertToBig5((DataBinder.Eval(Container.DataItem,"MasterName").ToString()),950)%></center>
				</ItemTemplate>
			</asp:TemplateColumn>
			<asp:TemplateColumn>
				<HeaderTemplate>
					<center>口號</center>
				</HeaderTemplate>
				<ItemTemplate>
					<%# sys.ConvertToBig5((DataBinder.Eval(Container.DataItem,"Intro").ToString()),950)%>
				</ItemTemplate>
			</asp:TemplateColumn>
			<asp:BoundColumn DataField="GuildLevel" HeaderText="等級"></asp:BoundColumn>
			<asp:BoundColumn DataField="GuildKillMonsterCount" HeaderText="殺怪數"></asp:BoundColumn>
			<asp:BoundColumn DataField="GuildPoint" HeaderText="幫會點數"></asp:BoundColumn>
		</Columns>
		<PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
	</asp:datagrid></FONT>

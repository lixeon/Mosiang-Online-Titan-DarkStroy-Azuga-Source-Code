namespace web.admin
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpagent : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected DataGrid DataGrid1;
        protected DropDownList ddnation;
        protected DropDownList ddstate;
        protected Label lblid;
        protected TextBox tbaddress;
        protected TextBox tbagentkey;
        protected TextBox tbbankinfo;
        protected TextBox tbgold;
        protected TextBox tbhandphone;
        protected TextBox tbmsn;
        protected TextBox tbname;
        protected TextBox tbother;
        protected TextBox tbpassword;
        protected TextBox tbqq;
        protected TextBox tbtelephone;
        protected TextBox tbuserid;

        private void btnadd_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = system.ChkSql(this.tbuserid.Text.ToString().Trim());
            string str2 = system.ChkSql(this.tbpassword.Text.ToString().Trim());
            string str3 = system.ChkSql(this.tbpassword.Text.ToString().Trim());
            string str4 = system.ChkSql(this.tbname.Text.ToString().Trim());
            string str5 = system.ChkSql(this.ddnation.SelectedValue.ToString().Trim());
            string str6 = system.ChkSql(this.tbqq.Text.ToString().Trim());
            string str7 = system.ChkSql(this.tbmsn.Text.ToString().Trim());
            string str8 = system.ChkSql(this.tbhandphone.Text.ToString().Trim());
            string str9 = system.ChkSql(this.tbtelephone.Text.ToString().Trim());
            string str10 = system.ChkSql(this.tbaddress.Text.ToString().Trim());
            string str11 = system.ChkSql(this.tbbankinfo.Text.ToString().Trim());
            string str12 = system.ChkSql(this.tbother.Text.ToString().Trim());
            int.Parse(this.tbgold.Text.ToString().Trim());
            int num = int.Parse(this.ddstate.SelectedValue.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { 
                "insert into mhcmember..web_agent (userid,password,agentkey,name,nation,qq,msn,handphone,telephone,address,bankinfo,other,state) values('", str, "','", str2, "','", str3, "','", str4, "','", str5, "','", str6, "','", str7, "','", str8, 
                "','", str9, "','", str10, "','", str11, "','", str12, "',", num, ")"
             }));
            base.Response.Redirect("cpagent.aspx");
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            system system = new system();
            int num = int.Parse(this.lblid.Text.ToString());
            system.ChkSql(this.tbuserid.Text.ToString().Trim());
            string str = system.ChkSql(this.tbpassword.Text.ToString().Trim());
            string str2 = system.ChkSql(this.tbpassword.Text.ToString().Trim());
            string str3 = system.ChkSql(this.tbname.Text.ToString().Trim());
            string str4 = system.ChkSql(this.ddnation.SelectedValue.ToString().Trim());
            string str5 = system.ChkSql(this.tbqq.Text.ToString().Trim());
            string str6 = system.ChkSql(this.tbmsn.Text.ToString().Trim());
            string str7 = system.ChkSql(this.tbhandphone.Text.ToString().Trim());
            string str8 = system.ChkSql(this.tbtelephone.Text.ToString().Trim());
            string str9 = system.ChkSql(this.tbaddress.Text.ToString().Trim());
            string str10 = system.ChkSql(this.tbbankinfo.Text.ToString().Trim());
            string str11 = system.ChkSql(this.tbother.Text.ToString().Trim());
            int num2 = int.Parse(this.tbgold.Text.ToString().Trim());
            int num3 = int.Parse(this.ddstate.SelectedValue.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { 
                "update mhcmember..web_agent set password='", str, "',agentkey='", str2, "',name='", str3, "',nation='", str4, "',qq='", str5, "',msn='", str6, "',handphone='", str7, "',telephone='", str8, 
                "',address='", str9, "',bankinfo='", str10, "',other='", str11, "',state=", num3, ",gold=", num2, " where id=", num
             }));
            base.Response.Redirect("cpagent.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            string str = new system().ChkSql(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            new DataProviders().ExecuteSql("delete from web_agent where userid='" + str + "'");
            base.Response.Redirect("cpagent.aspx");
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember..web_agent where userid='" + str + "'");
            if (reader.Read())
            {
                this.lblid.Text = reader["id"].ToString();
                this.tbuserid.Text = reader["userid"].ToString();
                this.tbpassword.Text = reader["password"].ToString();
                this.tbagentkey.Text = reader["agentkey"].ToString();
                this.tbname.Text = reader["name"].ToString();
                this.tbqq.Text = reader["qq"].ToString();
                this.tbmsn.Text = reader["msn"].ToString();
                this.tbhandphone.Text = reader["handphone"].ToString();
                this.tbtelephone.Text = reader["telephone"].ToString();
                this.tbaddress.Text = reader["address"].ToString();
                this.tbbankinfo.Text = reader["bankinfo"].ToString();
                this.tbother.Text = reader["other"].ToString();
                this.tbgold.Text = reader["gold"].ToString();
                this.ddnation.SelectedValue = reader["nation"].ToString();
                this.ddstate.SelectedValue = reader["state"].ToString();
                this.btnadd.Visible = false;
                this.btnedit.Visible = true;
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            this.btnadd.Click += new EventHandler(this.btnadd_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_agent", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}

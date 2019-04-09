namespace web.admin
{
    using System;
    using System.Data.SqlClient;
    using System.Web.Security;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpadmin : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected DataGrid DataGrid1;
        protected DropDownList ddstate;
        protected Label lblid;
        protected TextBox tbname;
        protected TextBox tbpassword;
        protected TextBox tbuserid;

        private void btnadd_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = system.ChkSql(this.tbuserid.Text.ToString().Trim());
            string str2 = FormsAuthentication.HashPasswordForStoringInConfigFile(system.ChkSql(this.tbpassword.Text.ToString().Trim()), "MD5");
            string str3 = system.ChkSql(this.tbname.Text.ToString().Trim());
            int num = int.Parse(this.ddstate.SelectedValue.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { "insert into web_login (userid,password,name,state) values('", str, "','", str2, "','", str3, "',", num, ")" }));
            base.Response.Redirect("cpadmin.aspx");
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str2 = "";
            system.ChkSql(this.tbuserid.Text.ToString().Trim());
            int num = int.Parse(this.lblid.Text.ToString());
            string password = system.ChkSql(this.tbpassword.Text.ToString().Trim());
            if (password != "")
            {
                password = FormsAuthentication.HashPasswordForStoringInConfigFile(password, "MD5");
                str2 = ",password='" + password + "'";
            }
            string str4 = system.ChkSql(this.tbname.Text.ToString().Trim());
            int num2 = int.Parse(this.ddstate.SelectedValue.ToString());
            string mySql = string.Concat(new object[] { "update web_login set state=", num2, ",name='", str4, "'", str2, " where id=", num });
            new DataProviders().ExecuteSql(mySql);
            base.Response.Redirect("cpadmin.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            string str = new system().ChkSql(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            new DataProviders().ExecuteSql("delete from web_login where id=" + str);
            base.Response.Redirect("cpadmin.aspx");
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_login where id=" + str);
            if (reader.Read())
            {
                this.lblid.Text = reader["id"].ToString();
                this.tbuserid.Text = reader["userid"].ToString();
                this.tbname.Text = reader["name"].ToString();
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
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_login", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}

namespace web.admin
{
    using System;
    using System.Data;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpmember : Page
    {
        protected Button btnedit;
        protected Button btnsearch;
        protected Button btnsearchcha;
        protected DataGrid DataGrid1;
        protected DropDownList ddlevel;
        protected DropDownList ddsex;
        protected DropDownList ddsweblevel;
        protected DropDownList ddvip;
        private static DataSet ds;
        protected static int rcount;
        private static string sql;
        protected TextBox tbcreatetime;
        protected TextBox tbpassword;
        protected TextBox tbschaname;
        protected TextBox tbscreatetimeend;
        protected TextBox tbscreatetimestart;
        protected TextBox tbsemail;
        protected TextBox tbskey;
        protected TextBox tbspassword;
        protected TextBox tbsuserid;
        protected TextBox tbsuseridx;
        protected TextBox tbswebbankmax;
        protected TextBox tbswebbankmin;
        protected TextBox tbswebgoldmax;
        protected TextBox tbswebgoldmin;
        protected TextBox tbswebregip;
        protected TextBox tbuserid;
        protected TextBox tbwebbank;
        protected TextBox tbwebemail;
        protected TextBox tbwebgold;
        protected TextBox tbwebkey;
        protected TextBox tbwebregip;

        private void btnedit_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = this.tbuserid.Text.ToString();
            string str2 = this.tbpassword.Text.ToString();
            string str3 = system.ChkSql(this.tbwebkey.Text.ToString());
            string str4 = system.ChkSql(this.tbwebemail.Text.ToString());
            int num = int.Parse(this.ddvip.SelectedValue.ToString());
            double num2 = double.Parse(this.tbwebbank.Text.ToString());
            double num3 = double.Parse(this.tbwebgold.Text.ToString());
            int num4 = int.Parse(this.ddlevel.SelectedValue.ToString());
            string mySql = string.Concat(new object[] { 
                "update mhcmember..chr_log_info set id_passwd='", str2, "',webemail='", str4, "',webkey='", str3, "',webbank=", num2, ",userlevel=", num4, ",weblevel=", num, ",webgold=", num3, " where id_loginid='", str, 
                "'"
             });
            new DataProviders().ExecuteSql(mySql);
        }

        private void btnsearch_Click(object sender, EventArgs e)
        {
            system system = new system();
            int num = int.Parse(this.tbsuseridx.Text.ToString());
            string str = system.ChkSql(this.tbsuserid.Text.ToString().Trim());
            string str2 = system.ChkSql(this.tbspassword.Text.ToString().Trim());
            string str3 = system.ChkSql(this.tbskey.Text.ToString().Trim());
            string str4 = system.ChkSql(this.tbsemail.Text.ToString().Trim());
            string str5 = system.ChkSql(this.tbswebregip.Text.ToString().Trim());
            string str6 = system.ChkSql(this.tbschaname.Text.ToString().Trim());
            int num2 = int.Parse(this.ddsweblevel.SelectedValue);
            double num3 = double.Parse(this.tbswebbankmin.Text.ToString().Trim());
            double num4 = double.Parse(this.tbswebbankmax.Text.ToString().Trim());
            double num5 = double.Parse(this.tbswebgoldmin.Text.ToString().Trim());
            double num6 = double.Parse(this.tbswebgoldmax.Text.ToString().Trim());
            string str7 = system.ChkSql(this.tbscreatetimestart.Text.ToString().Trim());
            string str8 = system.ChkSql(this.tbscreatetimeend.Text.ToString().Trim());
            sql = "select propid as user_idx,id_loginid as user_id,id_regdate as create_time ,* from mhcmember..chr_log_info  where";
            string str9 = "";
            int num7 = 0;
            if (num != 0)
            {
                if (num7 == 0)
                {
                    str9 = str9 + " propid = " + num;
                    num7++;
                }
                else
                {
                    str9 = str9 + " and  propid = " + num;
                }
            }
            if (str != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " id_loginid like '%" + str + "%'";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and id_loginid like '%" + str + "%'";
                }
            }
            if (str2 != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " id_passwd like '%" + str2 + "%'";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and id_passwd like '%" + str2 + "%'";
                }
            }
            if (str5 != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webregip like '%" + str5 + "%'";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webregip like '%" + str5 + "%'";
                }
            }
            if (str4 != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webemail like '%" + str4 + "%'";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webemail like '%" + str4 + "%'";
                }
            }
            if (str3 != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webkey like '%" + str3 + "%'";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webkey like '%" + str3 + "%'";
                }
            }
            if (num2 != 0x270f)
            {
                if (num7 == 0)
                {
                    str9 = str9 + " weblevel = " + num2;
                    num7++;
                }
                else
                {
                    str9 = str9 + " and weblevel = " + num2;
                }
            }
            if (num3 != 0.0)
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webbank >= " + num3;
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webbank >= " + num3;
                }
            }
            if (num4 != 0.0)
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webbank <= " + num4;
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webbank <= " + num4;
                }
            }
            if (num5 != 0.0)
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webgold >= " + num5;
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webgold >= " + num5;
                }
            }
            if (num6 != 0.0)
            {
                if (num7 == 0)
                {
                    str9 = str9 + " webgold <= " + num6;
                    num7++;
                }
                else
                {
                    str9 = str9 + " and webgold <= " + num6;
                }
            }
            if (str7 != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " id_regdate >= '" + str7 + "'";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and id_regdate >= '" + str7 + "'";
                }
            }
            if (str8 != "")
            {
                if (num7 == 0)
                {
                    str9 = str9 + " id_regdate <= '" + str8 + "'";
                }
                else
                {
                    str9 = str9 + " and id_regdate <= '" + str8 + "'";
                }
            }
            if (str6 != "")
            {
                str6 = new system().ConvertToBig5(str6, 0x3a8);
                if (num7 == 0)
                {
                    str9 = str9 + " propid in (select user_idx from mhgame..tb_character  where character_name like '%" + str6 + "%')";
                    num7++;
                }
                else
                {
                    str9 = str9 + " and propid in (select user_idx from mhgame..tb_character  where character_name like '%" + str6 + "%')";
                }
            }
            if (num7 == 0)
            {
                sql = sql.Replace("where", "");
            }
            sql = sql + str9;
            ds = new DataProviders().ExecuteSqlDs(sql, "DataGrid1");
            this.DataGrid1.DataSource = ds;
            this.DataGrid1.CurrentPageIndex = 0;
            this.DataGrid1.DataBind();
            rcount = ds.Tables[0].Rows.Count;
        }

        private void btnsearchcha_Click(object sender, EventArgs e)
        {
            base.Response.Write("<script>window.open('cpcharacter.aspx?userid=" + this.tbuserid.Text.ToString() + "')</script>");
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs(sql, "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember..chr_log_info  where propid=" + num);
            if (reader.Read())
            {
                this.tbuserid.Text = reader["id_loginid"].ToString();
                this.tbpassword.Text = reader["id_passwd"].ToString();
                this.tbwebkey.Text = reader["webkey"].ToString();
                this.tbwebemail.Text = reader["webemail"].ToString();
                this.tbwebgold.Text = reader["webgold"].ToString();
                this.tbwebbank.Text = reader["webbank"].ToString();
                this.tbcreatetime.Text = reader["id_regdate"].ToString();
                this.tbwebregip.Text = reader["webregip"].ToString();
                this.ddvip.SelectedValue = reader["weblevel"].ToString();
                this.ddsex.SelectedValue = reader["websex"].ToString();
                this.ddlevel.SelectedValue = reader["userlevel"].ToString();
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
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
            if ((!this.Page.IsPostBack && (base.Request.QueryString["useridx"] != null)) && (base.Request.QueryString["useridx"] != ""))
            {
                this.tbsuseridx.Text = base.Request.QueryString["useridx"].ToString();
            }
        }
    }
}

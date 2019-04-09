namespace web.agent
{
    using System;
    using System.Data;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class viewcharacter : Page
    {
        protected Button btnedit;
        protected Button btnsearch;
        protected Button btnsearchguild;
        protected Button btnsearuseridx;
        protected DataGrid DataGrid1;
        protected DropDownList ddmap;
        protected DropDownList ddsex;
        protected DropDownList ddstage;
        private static DataSet ds;
        private static DataSet mapds;
        protected static int rcount;
        private static string sql;
        protected TextBox tbabilityexp;
        protected TextBox tbbadfame;
        protected TextBox tbchaidx;
        protected TextBox tbchalv;
        protected TextBox tbchaname;
        protected TextBox tbchareset;
        protected TextBox tbdex;
        protected TextBox tbgengoal;
        protected TextBox tbguild;
        protected TextBox tblogouttime;
        protected TextBox tbmoney;
        protected TextBox tbnewname;
        protected TextBox tbparty;
        protected TextBox tbplaytime;
        protected TextBox tbpoint;
        protected TextBox tbsimmak;
        protected TextBox tbsta;
        protected TextBox tbsuserid;
        protected TextBox tbuseridx;

        private void btnsearch_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = system.ChkSql(this.tbsuserid.Text.ToString().Trim());
            sql = "select * from mhgame..tb_character where substring(character_name,1,1)!='@'";
            string str2 = "";
            if (str != "")
            {
                str2 = str2 + " and user_idx in(select propid from mhcmember..chr_log_info where id_loginid = '" + str + "')";
                sql = sql + str2;
                ds = new DataProviders().ExecuteSqlDs(sql, "DataGrid1");
                for (int i = 0; i < ds.Tables[0].Rows.Count; i++)
                {
                    ds.Tables[0].Rows[i]["character_name"] = system.ConvertToBig5(ds.Tables[0].Rows[i]["character_name"].ToString(), 950);
                }
                this.DataGrid1.DataSource = ds;
                this.DataGrid1.CurrentPageIndex = 0;
                this.DataGrid1.DataBind();
                rcount = ds.Tables[0].Rows.Count;
            }
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = ds;
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhgame..tb_character  where character_idx=" + num);
            if (reader.Read())
            {
                this.tbuseridx.Text = reader["user_idx"].ToString();
                this.tbchaidx.Text = reader["character_idx"].ToString();
                this.tbchaname.Text = new system().ConvertToBig5(reader["character_name"].ToString(), 950);
                this.tbguild.Text = new system().ConvertToBig5(reader["character_munpaname"].ToString(), 950);
                this.tbparty.Text = reader["character_partyidx"].ToString();
                if (this.tbguild.Text.ToString() == "")
                {
                    this.btnsearchguild.Enabled = false;
                }
                else
                {
                    this.btnsearchguild.Enabled = true;
                }
                this.ddsex.SelectedValue = reader["character_gender"].ToString();
                this.ddstage.SelectedValue = reader["character_stage"].ToString();
                this.ddmap.SelectedValue = reader["character_map"].ToString();
                this.tbchalv.Text = reader["character_grade"].ToString();
                this.tbmoney.Text = reader["character_money"].ToString();
                this.tbchareset.Text = reader["webchareset"].ToString();
                this.tbnewname.Text = reader["webnewname"].ToString();
                this.tbgengoal.Text = reader["character_gengoal"].ToString();
                this.tbsta.Text = reader["character_sta"].ToString();
                this.tbdex.Text = reader["character_dex"].ToString();
                this.tbsimmak.Text = reader["character_simmak"].ToString();
                this.tbpoint.Text = reader["character_gradeuppoint"].ToString();
                this.tbabilityexp.Text = reader["character_abilityexp"].ToString();
                this.tbbadfame.Text = reader["character_badfame"].ToString();
                this.tblogouttime.Text = reader["character_lastmodified"].ToString();
                this.tbplaytime.Text = reader["character_playtime"].ToString();
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isagent();
            if (base.Request.QueryString["userid"] != null)
            {
                this.tbsuserid.Text = base.Request.QueryString["userid"].ToString();
            }
            if (!this.Page.IsPostBack)
            {
                mapds = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_map_bin", "DataGrid1");
                this.ddmap.DataSource = mapds;
                this.ddmap.DataTextField = "name";
                this.ddmap.DataValueField = "id";
                this.ddmap.DataBind();
            }
        }
    }
}
